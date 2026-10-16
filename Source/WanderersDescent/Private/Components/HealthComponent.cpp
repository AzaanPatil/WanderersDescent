#include "Components/HealthComponent.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}

float UHealthComponent::ApplyDamage(float DamageAmount)
{
	if (bIsInvincible || IsDead() || DamageAmount <= 0.f)
	{
		return 0.f;
	}

	const float ActualDamage = FMath::Min(DamageAmount, CurrentHealth);
	CurrentHealth -= ActualDamage;
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);

	if (IsDead())
	{
		OnDeath.Broadcast();
	}

	return ActualDamage;
}

void UHealthComponent::Heal(float HealAmount)
{
	if (HealAmount <= 0.f || IsDead())
	{
		return;
	}

	CurrentHealth = FMath::Min(CurrentHealth + HealAmount, MaxHealth);
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}
