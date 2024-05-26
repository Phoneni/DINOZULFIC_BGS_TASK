// Fill out your copyright notice in the Description page of Project Settings.


#include "SkateMovementComponent.h"

void USkateMovementComponent::PhysWalking(float deltaTime, int32 Iterations)
{
	Super::PhysWalking(deltaTime, Iterations);

	const FVector OldVel = Velocity;
	FVector NewVel = Velocity.GetSafeNormal(0.01f);
	Velocity = FMath::VInterpTo(NewVel, GetOwner()->GetActorForwardVector(), deltaTime, VelocityInterpSpeed) * OldVel.Size();
}

float USkateMovementComponent::GetMaxSpeed() const
{
	if (MovementMode == MOVE_Walking || MovementMode == MOVE_NavWalking)
	{
		return static_cast<float>(CurrentStep) * MaxSpeedPerStep;
	}
	return Super::GetMaxSpeed();
}

void USkateMovementComponent::TryIncrementVelocityStep(float XDeltaTime)
{
	if (StepIncreaseElapsedTime > TimeToIncreaseStep)
	{
		if (CurrentStep < MaxSteps)
		{
			CurrentStep++;
		}
		StepIncreaseElapsedTime = 0.f;
	}
	else
	{
		StepIncreaseElapsedTime += XDeltaTime;
	}
}
