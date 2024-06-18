// Fill out your copyright notice in the Description page of Project Settings.


#include "Mover.h"
#include "CryptRaider/Trigger.h"

// Sets default values for this component's properties
UMover::UMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bIsTriggered = false;
}


// Called when the game starts
void UMover::BeginPlay()
{
	Super::BeginPlay();

	// Calulate the move speed for VInterpConstantTo
	ConstantMoveSpeed = MoveOffset.Size() / MoveTime;
	// Get the start location
	StartLocation = GetOwner()->GetActorLocation();

	UTrigger* Trigger = ActorWithTrigger->FindComponentByClass<UTrigger>();
	// Bind the TriggerMovement function to the OnTrigger delegate
	if (Trigger)
	{
		Trigger->OnTrigger.AddDynamic(this, &UMover::TriggerMovement);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Trigger component not found!"));
	}

	// UE_LOG(LogTemp, Warning, TEXT("MoveSpeed: %f"), ConstantMoveSpeed);
	// UE_LOG(LogTemp, Warning, TEXT("MoveOffset.Size(): %f"), MoveOffset.Size());
	// UE_LOG(LogTemp, Warning, TEXT("MoveTime: %f"), MoveTime);

	// A delay to trigger movement
	// FTimerHandle InvokeHandle; // Timer handle
	// Two ways to call TriggerMovement
	// GetWorld()->GetTimerManager().SetTimer(InvokeHandle, this, &UMover::TriggerMovement, 5, false);
	// GetWorld()->GetTimerManager().SetTimer(InvokeHandle, this, &UMover::TriggerMovement, 1, false, 5);
}


// Called every frame
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMover::TriggerMovement()
{
	if (bIsTriggered)
	{
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Triggering movement!"));
	// The target location
	FVector TargetLocation = GetOwner()->GetActorLocation() + MoveOffset;
	// Set the flag to indicate that the actor is moving
	bIsTriggered = true;

	// Lambda function to call MoveStep that uses VInterpConstantTo or VInterpTo
	auto MoveStepLambda = [this, TargetLocation]()
	{
		this->MoveStep(TargetLocation);
	};

	// Lambda function to call MoveStep that uses Lerp (not used)
	// Has to use a shared pointer, if declared DT as a float and captured by lambda as a reference, DT will be destroyed and out of scope after TriggerMovement is done
	TSharedPtr<float> DT = MakeShared<float>(0.0f);
	auto MoveStepLambda2 = [this, TargetLocation, DT]()
	{
		// UE_LOG(LogTemp, Warning, TEXT("Lambda called"));
		// *DT += 1;
		// UE_LOG(LogTemp, Warning, TEXT("Total DeltaTime: %f"), *DT);
		this->MoveStep(TargetLocation, DT);
	};

	// Start the timer to call MoveStep every frame
	StartTime = FPlatformTime::Seconds();
	if (MovementType == EMoverMode::Lerp)
	{
		GetWorld()->GetTimerManager().SetTimer(MoveTimerHandle, MoveStepLambda2, GetWorld()->GetDeltaSeconds(), true,
		                                       0);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(MoveTimerHandle, MoveStepLambda, GetWorld()->GetDeltaSeconds(), true, 0);
	}
}

// The version of MoveStep that uses VInterpConstantTo or VInterpTo
void UMover::MoveStep(const FVector& TargetLocation)
{
	AActor* Owner = GetOwner();
	FVector CurrentLocation = Owner->GetActorLocation();
	FVector NewLocation;

	switch (MovementType)
	{
		// Use VInterpTo
		case EMoverMode::VInterpTo:
			NewLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, GetWorld()->GetDeltaSeconds(), 5);
			break;

		// Use VInterpConstantTo (default)
		case EMoverMode::VInterpConstantTo:
		default:
			NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, GetWorld()->GetDeltaSeconds(),
			                                       ConstantMoveSpeed);
			break;
	}

	Owner->SetActorLocation(NewLocation);
	if (NewLocation.Equals(TargetLocation, 0.1f))
	{
		// Temp, for timer
		double EndTime = FPlatformTime::Seconds();
		UE_LOG(LogTemp, Warning, TEXT("Time taken: %f"), EndTime - StartTime);

		// bIsTriggered = false;
		GetWorld()->GetTimerManager().ClearTimer(MoveTimerHandle);
		UE_LOG(LogTemp, Warning, TEXT("Movement complete"));
	}
}


// The version of MoveStep that uses Lerp
void UMover::MoveStep(const FVector& TargetLocation, TSharedPtr<float> DeltaTime)
{
	AActor* Owner = GetOwner();
	*DeltaTime += GetWorld()->GetDeltaSeconds();
	float Alpha = FMath::Clamp(*DeltaTime / MoveTime, 0.0f, 1.0f);
	FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, Alpha);
	Owner->SetActorLocation(NewLocation);
	if (Alpha >= 1.0f)
	{
		// Temp
		double EndTime = FPlatformTime::Seconds();
		UE_LOG(LogTemp, Warning, TEXT("Time taken: %f"), EndTime - StartTime);
		bIsTriggered = false;
		GetWorld()->GetTimerManager().ClearTimer(MoveTimerHandle);
		UE_LOG(LogTemp, Warning, TEXT("Movement complete"));
	}
}
