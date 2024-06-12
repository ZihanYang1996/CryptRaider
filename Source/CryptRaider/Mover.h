// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Mover.generated.h"

// Define the enum for the movement type
UENUM(BlueprintType)
enum class EMoverMode: uint8
{
	Lerp UMETA(DisplayName = "Lerp"),
	VInterpTo UMETA(DisplayName = "VInterpTo"),
	VInterpConstantTo UMETA(DisplayName = "VInterpConstantTo")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRYPTRAIDER_API UMover : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMover();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void TriggerMovement();
	
private:
	UPROPERTY(EditAnywhere, Category="MovingSettings")
	FVector MoveOffset;

	UPROPERTY(EditAnywhere, Category="MovingSettings", meta=(Tooltip="The time it takes to move for constant speed."))
	float MoveTime = 2.0f;

	UPROPERTY(EditAnywhere, Category="MovingSettings", meta=(Tooltip="Choose the movement type."))
	EMoverMode MovementType = EMoverMode::VInterpConstantTo;
	
	UPROPERTY(EditAnywhere, Category="MovingSettings", meta=(Tooltip="The speed when using VInterpTo."))
	float MoveSpeed = 5.0f;
	
	bool bIsMoving = false;
	float ConstantMoveSpeed;
	FTimerHandle MoveTimerHandle;
	FVector StartLocation;

	// Temp, for timer
	double StartTime;

	void MoveStep(const FVector& TargetLocation);
	void MoveStep(const FVector& TargetLocation, TSharedPtr<float> DeltaTime);
};
