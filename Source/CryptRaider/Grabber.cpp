// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Below are moved to the Grab function
	
	// FVector Start = GetComponentLocation();
	// FVector End = Start + GetForwardVector() * MaxGrabDistance;
	// DrawDebugLine(GetWorld(), Start, End, FColor::Red, false);

	// FHitResult HitResult;
	// FCollisionShape CollisionShape = FCollisionShape::MakeSphere(GrabRadius);
	// bool bHasHit = GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel2, CollisionShape);

	// if (bHasHit)
	// {
	// 	// Get the actor that was hit
	// 	AActor* HitActor = HitResult.GetActor();
	// 	// Calculate distance to the hit
	// 	// float Distance = HitResult.Distance;
	// 	// UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *HitActor->GetActorNameOrLabel());
	// 	// UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), Distance);
	// }
	// else
	// {
	// 	// UE_LOG(LogTemp, Warning, TEXT("No hit"));
	// }
	
}

void UGrabber::Grab(const FInputActionValue& Value)
{
	// input is a bool
	bool bIsGrabbing = Value.Get<bool>();

	// Check if the GEngine is available first, print an error message and return if it's not
	if (!GEngine)
	{
		UE_LOG(LogTemp, Error, TEXT("No GEngine"));
		return;
	}

	if (bIsGrabbing)
	{		
		FVector Start = GetComponentLocation();
		FVector End = Start + GetForwardVector() * MaxGrabDistance;
		
		FHitResult HitResult;
		FCollisionShape CollisionShape = FCollisionShape::MakeSphere(GrabRadius);
		
		bool bHasHit = GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel2, CollisionShape);

		if (bHasHit)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Grabbed"));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Grabbing failed"));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Released"));
	}
	
}

