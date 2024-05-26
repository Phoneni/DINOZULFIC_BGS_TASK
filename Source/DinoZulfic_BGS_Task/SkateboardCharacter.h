
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"
#include "SkateboardCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;


UCLASS()
class DINOZULFIC_BGS_TASK_API ASkateboardCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASkateboardCharacter();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CharacterSpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CharacterCamera;

	UPROPERTY(EditDefaultsOnly, Category = "Controls | Input")
	UInputAction* MovementAction;
	UPROPERTY(EditDefaultsOnly, Category = "Controls | Input")
	UInputAction* SteeringAction;
	UPROPERTY(EditDefaultsOnly, Category = "Controls | Input")
	UInputAction* JumpAction;
	UPROPERTY(EditDefaultsOnly, Category = "Controls | Input")
	UInputAction* CameraMovementAction;

	UPROPERTY(EditDefaultsOnly, Category = "Controls | Input")
	UInputMappingContext* BaseMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float MaxMovementSpeed;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float AccelerationModifier;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float DecelrationModifier;



protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PawnClientRestart() override;


private:

	void AccelerateFn(float InValue);
	void DecelerateFn(float InValue);
	void SteerFn(float InValue);


	UFUNCTION()
	void MovementFn(const FInputActionValue& MovementValue);
	UFUNCTION()
	void JumpFn(const FInputActionValue& JumpValue);
	UFUNCTION()
	void CameraFn(const FInputActionValue& CameraValue);

};
