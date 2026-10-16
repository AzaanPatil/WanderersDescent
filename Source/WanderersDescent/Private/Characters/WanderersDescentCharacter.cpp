#include "Characters/WanderersDescentCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystem.h"
#include "Components/StaminaComponent.h"
#include "Components/CombatComponent.h"
#include "Components/PowerUpComponent.h"
#include "Components/HealthComponent.h"
#include "Engine/Engine.h"
#include "WanderersDescent.h"

AWanderersDescentCharacter::AWanderersDescentCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	StaminaComponent = CreateDefaultSubobject<UStaminaComponent>(TEXT("StaminaComponent"));
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	PowerUpComponent = CreateDefaultSubobject<UPowerUpComponent>(TEXT("PowerUpComponent"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

void AWanderersDescentCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}

void AWanderersDescentCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	if (MoveAction)
	{
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AWanderersDescentCharacter::Move);
	}
	if (LookAction)
	{
		EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &AWanderersDescentCharacter::Look);
	}
	if (JumpAction)
	{
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	}
	if (LightAttackAction)
	{
		EnhancedInput->BindAction(LightAttackAction, ETriggerEvent::Started, this, &AWanderersDescentCharacter::LightAttack);
	}
	if (HeavyAttackAction)
	{
		EnhancedInput->BindAction(HeavyAttackAction, ETriggerEvent::Started, this, &AWanderersDescentCharacter::HeavyAttack);
	}
	if (DodgeAction)
	{
		EnhancedInput->BindAction(DodgeAction, ETriggerEvent::Started, this, &AWanderersDescentCharacter::Dodge);
	}

	for (int32 SlotIndex = 0; SlotIndex < PowerUpActions.Num(); ++SlotIndex)
	{
		if (UInputAction* Action = PowerUpActions[SlotIndex])
		{
			EnhancedInput->BindAction(Action, ETriggerEvent::Started, this, &AWanderersDescentCharacter::ActivatePowerUp, SlotIndex);
		}
	}
}

void AWanderersDescentCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (!Controller)
	{
		return;
	}

	const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void AWanderersDescentCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (!Controller)
	{
		return;
	}

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AWanderersDescentCharacter::LightAttack(const FInputActionValue& Value)
{
	if (CombatComponent)
	{
		const bool bSucceeded = CombatComponent->TryLightAttack();
		DebugInput(FString::Printf(TEXT("Light attack: %s"), bSucceeded ? TEXT("OK") : TEXT("blocked (busy or no stamina)")));
	}
}

void AWanderersDescentCharacter::HeavyAttack(const FInputActionValue& Value)
{
	if (CombatComponent)
	{
		const bool bSucceeded = CombatComponent->TryHeavyAttack();
		DebugInput(FString::Printf(TEXT("Heavy attack: %s"), bSucceeded ? TEXT("OK") : TEXT("blocked (busy or no stamina)")));
	}
}

void AWanderersDescentCharacter::Dodge(const FInputActionValue& Value)
{
	if (CombatComponent)
	{
		const bool bSucceeded = CombatComponent->TryDodge();
		DebugInput(FString::Printf(TEXT("Dodge: %s"), bSucceeded ? TEXT("OK") : TEXT("blocked (busy or no stamina)")));
	}
}

void AWanderersDescentCharacter::ActivatePowerUp(int32 SlotIndex)
{
	if (PowerUpComponent)
	{
		const bool bSucceeded = PowerUpComponent->TryActivate(SlotIndex);
		DebugInput(FString::Printf(TEXT("Power-up slot %d: %s"), SlotIndex, bSucceeded ? TEXT("OK") : TEXT("blocked (cooldown or empty slot)")));
	}
}

// Temporary: makes input visible in PIE until animations exist. Remove once Week 2 animation work lands.
void AWanderersDescentCharacter::DebugInput(const FString& Message) const
{
	const float Stamina = StaminaComponent ? StaminaComponent->CurrentStamina : 0.f;
	const FString FullMessage = FString::Printf(TEXT("%s  [stamina %.0f]"), *Message, Stamina);

	UE_LOG(LogWanderersDescent, Log, TEXT("%s"), *FullMessage);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FullMessage);
	}
}
