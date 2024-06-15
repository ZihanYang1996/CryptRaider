// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Components/SceneComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRYPTRAIDER_API UGrabber : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	/** The Physics Handle component */
	class UPhysicsHandleComponent* PhysicsHandle; // Forward declaration of the Physics Handle component

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Ray-cast and grab what's in reach
	void Grab(const FInputActionValue& Value);

private:
	UPROPERTY(EditAnywhere)
	float MaxGrabDistance = 100.0f;

	UPROPERTY(EditAnywhere)
	float GrabRadius = 10.0f;

	UPROPERTY(EditAnywhere)
	float HoldDistance = 100.0f;
};
