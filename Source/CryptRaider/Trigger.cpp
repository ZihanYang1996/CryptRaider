// Fill out your copyright notice in the Description page of Project Settings.


#include "Trigger.h"
// #include "Mover.h"

UTrigger::UTrigger()
{
	OnComponentEndOverlap.AddDynamic(this, &UTrigger::OnOverlapEnd);
}

void UTrigger::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlap ended!"));

	// Using the Mover component to move the actor
	// UMover* Mover = ActorToMove->FindComponentByClass<UMover>();
	// if (Mover)
	// {
	// 	Mover->TriggerMovement();
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("Mover component not found!"));
	// }

	// Using the delegate
	OnTrigger.Broadcast();
}
