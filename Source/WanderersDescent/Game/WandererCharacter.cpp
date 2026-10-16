#include "WandererCharacter.h"
#include "StaminaComponent.h"
#include "PowerUpComponent.h"
#include "WanderersDescent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Engine/Engine.h"
#include "TimerManager.h"

AWandererCharacter::AWandererCharacter()
{
	Stamina = CreateDefaultSubobject<UStaminaComponent>(TEXT("Stamina"));
	PowerUps = CreateDefaultSubobject<UPowerUpComponent>(TEXT("PowerUps"));
}

void AWandererCharacter::BeginPlay()
{
	Super::BeginPlay();

	WalkSpeed = GetCharacterMovement()->MaxWalkSpeed;

	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->OnMontageStarted.AddDynamic(this, &AWandererCharacter::HandleMontageStarted);
	}
}

void AWandererCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearTimer(DodgeTimer);
	GetWorld()->GetTimerManager().ClearTimer(InvincibilityTimer);

	Super::EndPlay(EndPlayReason);
}

void AWandererCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInput)
	{
		return;
	}

	if (DodgeAction)
	{
		EnhancedInput->BindAction(DodgeAction, ETriggerEvent::Started, this, &AWandererCharacter::DoDodge);
	}

	if (SprintAction)
	{
		EnhancedInput->BindAction(SprintAction, ETriggerEvent::Started, this, &AWandererCharacter::DoSprintStart);
		EnhancedInput->BindAction(SprintAction, ETriggerEvent::Completed, this, &AWandererCharacter::DoSprintEnd);
	}

	for (int32 SlotIndex = 0; SlotIndex < PowerUpActions.Num(); ++SlotIndex)
	{
		if (PowerUpActions[SlotIndex])
		{
			EnhancedInput->BindAction(PowerUpActions[SlotIndex], ETriggerEvent::Started, this, &AWandererCharacter::DoActivatePowerUp, SlotIndex);
		}
	}
}

void AWandererCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const bool bIsMovingOnGround = GetVelocity().SizeSquared2D() > 1.f && !GetCharacterMovement()->IsFalling();
	if (bIsSprinting && bIsMovingOnGround && !Stamina->TrySpend(SprintStaminaPerSecond * DeltaSeconds))
	{
		DoSprintEnd();
		DebugMessage(TEXT("Sprint stopped: out of stamina"));
	}
}

void AWandererCharacter::DoComboAttackStart()
{
	if (bIsDodging)
	{
		return;
	}

	if (!Stamina->CanAfford(LightAttackStaminaCost))
	{
		DebugMessage(TEXT("Light attack blocked: not enough stamina"));
		return;
	}

	Super::DoComboAttackStart();
}

void AWandererCharacter::DoChargedAttackStart()
{
	if (bIsDodging)
	{
		return;
	}

	if (!Stamina->CanAfford(HeavyAttackStaminaCost))
	{
		DebugMessage(TEXT("Heavy attack blocked: not enough stamina"));
		return;
	}

	Super::DoChargedAttackStart();
}

void AWandererCharacter::CheckCombo()
{
	// Mirrors the condition ACombatCharacter::CheckCombo uses to continue the string.
	const bool bWantsNextHit = bIsAttacking && !bIsChargingAttack
		&& GetWorld()->GetTimeSeconds() - CachedAttackInputTime <= ComboInputCacheTimeTolerance;

	if (bWantsNextHit && !Stamina->CanAfford(LightAttackStaminaCost))
	{
		// Drop the buffered input so the combo ends here instead of continuing for free.
		CachedAttackInputTime = 0.f;
		DebugMessage(TEXT("Combo ended: not enough stamina"));
	}

	const int32 PreviousComboCount = ComboCount;
	Super::CheckCombo();

	if (ComboCount > PreviousComboCount && ComboCount < ComboSectionNames.Num())
	{
		Stamina->TrySpend(LightAttackStaminaCost);
		DebugMessage(FString::Printf(TEXT("Combo hit %d"), ComboCount + 1));
	}
}

void AWandererCharacter::HandleMontageStarted(UAnimMontage* Montage)
{
	if (Montage == ComboAttackMontage)
	{
		Stamina->TrySpend(LightAttackStaminaCost);
		DebugMessage(TEXT("Light attack"));
	}
	else if (Montage == ChargedAttackMontage)
	{
		Stamina->TrySpend(HeavyAttackStaminaCost);
		DebugMessage(TEXT("Heavy attack"));
	}
}

void AWandererCharacter::DoDodge()
{
	if (bIsDodging || bIsAttacking || CurrentHP <= 0.f || GetCharacterMovement()->IsFalling())
	{
		return;
	}

	if (!Stamina->TrySpend(DodgeStaminaCost))
	{
		DebugMessage(TEXT("Dodge blocked: not enough stamina"));
		return;
	}

	// Dodge the way the player is steering; with no input, hop backwards.
	FVector Direction = GetLastMovementInputVector().GetSafeNormal2D();
	if (Direction.IsNearlyZero())
	{
		Direction = -GetActorForwardVector();
	}
	else
	{
		SetActorRotation(Direction.Rotation());
	}

	LaunchCharacter(Direction * DodgeSpeed, true, false);

	if (DodgeMontage)
	{
		PlayAnimMontage(DodgeMontage);
	}

	bIsDodging = true;
	bIsInvincible = true;

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.SetTimer(DodgeTimer, this, &AWandererCharacter::EndDodge, DodgeDuration, false);
	TimerManager.SetTimer(InvincibilityTimer, this, &AWandererCharacter::EndInvincibility, DodgeInvincibilityDuration, false);

	DebugMessage(TEXT("Dodge"));
}

void AWandererCharacter::EndDodge()
{
	bIsDodging = false;
}

void AWandererCharacter::EndInvincibility()
{
	bIsInvincible = false;
}

void AWandererCharacter::DoSprintStart()
{
	bIsSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void AWandererCharacter::DoSprintEnd()
{
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AWandererCharacter::DoActivatePowerUp(int32 SlotIndex)
{
	if (PowerUps->TryActivate(SlotIndex))
	{
		DebugMessage(FString::Printf(TEXT("Power-up %d: %s"), SlotIndex + 1, *PowerUps->PowerUpSlots[SlotIndex].ToString()));
	}
	else if (!PowerUps->PowerUpSlots.IsValidIndex(SlotIndex))
	{
		DebugMessage(FString::Printf(TEXT("Power-up %d: no power-up in this slot"), SlotIndex + 1));
	}
	else
	{
		DebugMessage(FString::Printf(TEXT("Power-up %d: on cooldown (%.1fs)"), SlotIndex + 1, PowerUps->GetCooldownRemaining()));
	}
}

float AWandererCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsInvincible)
	{
		DebugMessage(TEXT("Hit avoided (i-frames)"));
		return 0.f;
	}

	return Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
}

void AWandererCharacter::DebugMessage(const FString& Message) const
{
	if (!bShowDebugMessages)
	{
		return;
	}

	const FString FullMessage = FString::Printf(TEXT("%s  [stamina %.0f]"), *Message, Stamina->CurrentStamina);
	UE_LOG(LogWanderersDescent, Log, TEXT("%s"), *FullMessage);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FullMessage);
	}
}
