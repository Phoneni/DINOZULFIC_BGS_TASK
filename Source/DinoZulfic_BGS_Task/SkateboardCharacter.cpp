

#include "SkateboardCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"




ASkateboardCharacter::ASkateboardCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	CharacterSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CharacterSpringArm"));
	CharacterSpringArm->SetupAttachment(RootComponent);

	CharacterCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CharacterCamera"));
	CharacterCamera->SetupAttachment(CharacterSpringArm);
}

void ASkateboardCharacter::BeginPlay()
{
	Super::BeginPlay();	
}

void ASkateboardCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASkateboardCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MovementAction) { EIC->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ASkateboardCharacter::MovementFn); }
		if (JumpAction) { EIC->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASkateboardCharacter::JumpFn); }
		if (CameraMovementAction) { EIC->BindAction(CameraMovementAction, ETriggerEvent::Triggered, this, &ASkateboardCharacter::CameraFn); }
	}
}

void ASkateboardCharacter::PawnClientRestart()
{
	Super::PawnClientRestart();

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(BaseMappingContext, 0);
		}
	}
}

void ASkateboardCharacter::AccelerateFn(float InValue)
{
	if (InValue >= 0)
	{
		AddMovementInput(InValue * GetActorForwardVector());
	}
	else if (InValue < 0)
	{
		DecelerateFn(InValue);
	}
}

void ASkateboardCharacter::SteerFn(float InValue)
{
	AddMovementInput(InValue * GetActorRightVector());
}


void ASkateboardCharacter::DecelerateFn(float InValue)
{

}


void ASkateboardCharacter::MovementFn(const FInputActionValue& MovementValue)
{
	FVector CamForward = CharacterCamera->GetForwardVector();
	CamForward.Z = 0.f;
	CamForward = CamForward.GetSafeNormal(0.001f);

	float AccelerateValue = GetActorForwardVector().ProjectOnTo(CamForward).Size() * MovementValue[1];
	float SteerValue = GetActorRightVector().ProjectOnTo(CamForward).Size() * MovementValue[0];
	AccelerateFn(AccelerateValue);
	SteerFn(SteerValue);
}

void ASkateboardCharacter::JumpFn(const FInputActionValue& JumpValue)
{
	const bool JumpVal = JumpValue.Get<bool>();
	if (GetController())
	{
		if (JumpVal)
		{
			Jump();
		}
		else
		{
			StopJumping();
		}
	}
}

void ASkateboardCharacter::CameraFn(const FInputActionValue& CameraValue)
{
	const FVector2D CamValue = CameraValue.Get<FVector2D>();
	if (GetController())
	{
		AddControllerPitchInput(CamValue.Y);
		AddControllerYawInput(CamValue.X);
	}
}

