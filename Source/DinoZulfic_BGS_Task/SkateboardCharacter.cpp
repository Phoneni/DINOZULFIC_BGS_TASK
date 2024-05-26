

#include "SkateboardCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SkateMovementComponent.h"

ASkateboardCharacter::ASkateboardCharacter(const FObjectInitializer& OI)
	:
	Super(OI.SetDefaultSubobjectClass<USkateMovementComponent>(ACharacter::CharacterMovementComponentName)),
	RagdollDuration(4.f),
	RagdollElapsedTime(0.f)
{
	PrimaryActorTick.bCanEverTick = true;
	bCanJump = true;
	JumpCooldownDuration = 0.7f;

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

	if (bApplyingMovement)
	{
		GetSkateMovementComponent()->TryIncrementVelocityStep(DeltaTime);
	}

	if (bRagdolling)
	{
		RagdollElapsedTime += DeltaTime;
		if (RagdollElapsedTime > RagdollDuration)
		{
			GetMesh()->SetSimulatePhysics(false);
		}
		//SetActorLocation(GetMesh()->GetComponentLocation());
	}
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

USkateMovementComponent* ASkateboardCharacter::GetSkateMovementComponent()
{
	if (!SkateMovementRef.IsValid())
	{
		SkateMovementRef = Cast<USkateMovementComponent>(GetCharacterMovement());
	}
	return SkateMovementRef.Get();
}

void ASkateboardCharacter::StartRagdoll()
{
	RagdollElapsedTime = 0.f;
	bRagdolling = true;
	GetMesh()->SetSimulatePhysics(true);
}

void ASkateboardCharacter::EnableJump()
{
	bCanJump = true;
}

void ASkateboardCharacter::Landed(const FHitResult& Hit)
{
	bCanJump = false;
	GetWorld()->GetTimerManager().SetTimer(JumpCooldownTimerHandle, this, &ASkateboardCharacter::EnableJump, JumpCooldownDuration, false);

	const float CameraRightDot = FVector::DotProduct(GetActorRightVector(), GetVelocity().GetSafeNormal(0.01f));
	if (CameraRightDot > 0.707f || CameraRightDot < -0.707f)
	{
		StartRagdoll();
	}
}

void ASkateboardCharacter::MovementFn(const FInputActionValue& MovementValue)
{
	bApplyingMovement = MovementValue[1] > 0.f;
	if (bApplyingMovement)
	{
		
		FVector CamForward = CharacterCamera->GetForwardVector();
		CamForward.Z = 0.f;
		CamForward = CamForward.GetSafeNormal(0.001f);
		AddMovementInput(CamForward * MovementValue[1]);
	}
}

void ASkateboardCharacter::JumpFn(const FInputActionValue& JumpValue)
{
	const bool JumpVal = JumpValue.Get<bool>();
	if (GetController())
	{
		if (JumpVal && bCanJump)
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

