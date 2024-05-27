
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"
#include "SkateboardCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class USkateMovementComponent;

UCLASS()
class DINOZULFIC_BGS_TASK_API ASkateboardCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASkateboardCharacter(const FObjectInitializer& OI);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CharacterSpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CharacterCamera;

	UPROPERTY(EditDefaultsOnly, Category = "Controls | Input")
	UInputAction* MovementAction;
	UPROPERTY(EditDefaultsOnly, Category = "Controls | Input")
	UInputAction* JumpAction;
	UPROPERTY(EditDefaultsOnly, Category = "Controls | Input")
	UInputAction* CameraMovementAction;

	UPROPERTY(EditDefaultsOnly, Category = "Controls | Input")
	UInputMappingContext* BaseMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Ragdoll")
	float RagdollDuration;

private:
	FTimerHandle JumpCooldownTimerHandle;
	float LastFrameSpeed;
	bool bCanJump;
	float JumpCooldownDuration;
	float RagdollElapsedTime;
	bool bApplyingMovement;
	TWeakObjectPtr<USkateMovementComponent> SkateMovementRef;
	bool bRagdolling;
	FTransform MeshOriginalTransform;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PawnClientRestart() override;
	virtual void Landed(const FHitResult& Hit) override;
	UFUNCTION(BlueprintPure)
	USkateMovementComponent* GetSkateMovementComponent();
	UFUNCTION(BlueprintImplementableEvent)
	void PostIncrementedStepCount(int InInt);

protected:
	virtual void BeginPlay() override;


private:
	void StartRagdoll();
	void EnableJump();


	UFUNCTION()
	void MovementFn(const FInputActionValue& MovementValue);
	UFUNCTION()
	void JumpFn(const FInputActionValue& JumpValue);
	UFUNCTION()
	void CameraFn(const FInputActionValue& CameraValue);

};
