// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Trigger.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTrigger);

UCLASS()
class CRYPTRAIDER_API UTrigger : public UBoxComponent
{
	GENERATED_BODY()

public:
	UTrigger();
	
	FOnTrigger OnTrigger;

private:
	UPROPERTY(EditAnywhere, Category="Actor to move")
	AActor* ActorToMove;

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
};
