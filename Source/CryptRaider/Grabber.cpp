// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"


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

	// Set up the PhysicsHandle component
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Type::Error, TEXT("PhysicsHandle component not found!"));
	}
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Move the grabbed component to the hold location
	if (PhysicsHandle->GetGrabbedComponent())
	{
		FVector HoldLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
		PhysicsHandle->SetTargetLocationAndRotation(HoldLocation, GetComponentRotation());
	}

	
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

bool UGrabber::GetGrabbableInReach(FHitResult& HitResult) const
{
	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * MaxGrabDistance;

	// Draw a debug sphere at the end of the sweep (line trace)
	// DrawDebugSphere(GetWorld(), End, 10, 10, FColor::Blue, false, 5.0f);

	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(GrabRadius);
		
	return GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel2, CollisionShape);
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
		FHitResult HitResult;
		bool bHasHit = GetGrabbableInReach(HitResult);

		if (bHasHit)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Grabbed"));
			// HitRsult.Location is the center of the sphere when the sweep hits, so will use ImpactPoint instead
			// DrawDebugSphere(GetWorld(), HitResult.Location, 10, 10, FColor::Green, false, 5.0f);
			
			// HitResult.ImpactPoint is the exact point where the sweep hits
			DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 10, FColor::Red, false, 5.0f);

			// Grab the component at the impact point
			// First wake up the hit component
			HitResult.GetComponent()->WakeRigidBody();
			PhysicsHandle->GrabComponentAtLocationWithRotation(HitResult.GetComponent(), NAME_None, HitResult.ImpactPoint, GetComponentRotation());
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Grabbing failed"));
		}
	}
	else
	{
		// Check if an object is grabbed
		if (!PhysicsHandle->GetGrabbedComponent())
		{
			return;
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Released"));
		// Release the grabbed component
		PhysicsHandle->ReleaseComponent();
	}
	
}

