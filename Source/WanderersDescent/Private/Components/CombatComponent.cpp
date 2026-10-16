#include "Components/CombatComponent.h"
#include "Components/StaminaComponent.h"
#include "Components/HealthComponent.h"
#include "TimerManager.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	if (AActor* Owner = GetOwner())
	{
		StaminaComponent = Owner->FindComponentByClass<UStaminaComponent>();
		HealthComponent = Owner->FindComponentByClass<UHealthComponent>();
	}
}

bool UCombatComponent::TryLightAttack()
{
	if (IsBusy() || !StaminaComponent || !StaminaComponent->TrySpend(LightAttackStaminaCost))
	{
		return false;
	}

	EnterState(ECombatState::LightAttack, LightAttackDuration);
	return true;
}

bool UCombatComponent::TryHeavyAttack()
{
	if (IsBusy() || !StaminaComponent || !StaminaComponent->TrySpend(HeavyAttackStaminaCost))
	{
		return false;
	}

	EnterState(ECombatState::HeavyAttack, HeavyAttackDuration);
	return true;
}

bool UCombatComponent::TryDodge()
{
	if (IsBusy() || !StaminaComponent || !StaminaComponent->TrySpend(DodgeStaminaCost))
	{
		return false;
	}

	EnterState(ECombatState::Dodging, DodgeDuration);
	BeginDodgeInvincibility();
	return true;
}

void UCombatComponent::EnterState(ECombatState NewState, float Duration)
{
	CombatState = NewState;

	GetWorld()->GetTimerManager().ClearTimer(StateTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(StateTimerHandle, this, &UCombatComponent::ReturnToIdle, Duration, false);
}

void UCombatComponent::ReturnToIdle()
{
	CombatState = ECombatState::Idle;
}

void UCombatComponent::BeginDodgeInvincibility()
{
	if (HealthComponent)
	{
		HealthComponent->bIsInvincible = true;
	}

	GetWorld()->GetTimerManager().ClearTimer(InvincibilityTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(InvincibilityTimerHandle, this, &UCombatComponent::EndDodgeInvincibility, DodgeInvincibleDuration, false);
}

void UCombatComponent::EndDodgeInvincibility()
{
	if (HealthComponent)
	{
		HealthComponent->bIsInvincible = false;
	}
}
