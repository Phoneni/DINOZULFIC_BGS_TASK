// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SkateMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class DINOZULFIC_BGS_TASK_API USkateMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	float VelocityInterpSpeed = 3.f;
	UPROPERTY(EditDefaultsOnly)
	float MaxSpeedPerStep = 400.f;
	UPROPERTY(EditDefaultsOnly)
	int32 MaxSteps = 3;
	UPROPERTY(EditDefaultsOnly)
	float TimeToIncreaseStep = 1.f;

private:
	int32 CurrentStep = 0;
	float StepIncreaseElapsedTime = BIG_NUMBER;
	
public:
	virtual void PhysWalking(float deltaTime, int32 Iterations) override;
	virtual float GetMaxSpeed() const override;
	void TryIncrementVelocityStep(float XDeltaTime);
};
