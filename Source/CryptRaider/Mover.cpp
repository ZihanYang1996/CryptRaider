// Fill out your copyright notice in the Description page of Project Settings.


#include "Mover.h"

// Sets default values for this component's properties
UMover::UMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bIsMoving = false;
	
	// ...
}


// Called when the game starts
void UMover::BeginPlay()
{
	Super::BeginPlay();

	// ...

	MoveSpeed = MoveOffset.Size() / MoveTime;
	// Log MoveSpeed
	UE_LOG(LogTemp, Warning, TEXT("MoveSpeed: %f"), MoveSpeed);
	UE_LOG(LogTemp, Warning, TEXT("MoveOffset.Size(): %f"), MoveOffset.Size());
	UE_LOG(LogTemp, Warning, TEXT("MoveTime: %f"), MoveTime);

	// A delay to trigger movement
	FTimerHandle InvokeHandle;
	GetWorld()->GetTimerManager().SetTimer(InvokeHandle, this, &UMover::TriggerMovement, 5, false);
	// TriggerMovement();
}


// Called every frame
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMover::TriggerMovement()
{
	UE_LOG(LogTemp, Warning, TEXT("Triggering movement!"));
	FVector TargetLocation = GetOwner()->GetActorLocation() + MoveOffset;
	bIsMoving = true;

	// Lambda function to call MoveStep with parameters
	auto MoveStepLambda = [this, TargetLocation]()
	{
		UE_LOG(LogTemp, Warning, TEXT("Lambda called"));
		this->MoveStep(TargetLocation);
	};

	// Start the timer to call MoveStep every frame
	GetWorld()->GetTimerManager().SetTimer(MoveTimerHandle, MoveStepLambda, 0.01f, true);
}

void UMover::MoveStep(const FVector& TargetLocation)
{
	AActor* Owner = GetOwner();
	FVector CurrentLocation = Owner->GetActorLocation();
	FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, GetWorld()->GetDeltaSeconds(), MoveSpeed);
	
	
	UE_LOG(LogTemp, Warning, TEXT("NewLocation location: %s"), *NewLocation.ToString());

	Owner->SetActorLocation(NewLocation);
	if (NewLocation.Equals(TargetLocation, 0.1f))
	{
		bIsMoving = false;
		GetWorld()->GetTimerManager().ClearTimer(MoveTimerHandle);
		UE_LOG(LogTemp, Warning, TEXT("Movement complete"));
	}
}
