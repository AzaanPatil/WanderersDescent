#include "BossCharacter.h"
#include "WanderersDescent.h"

ABossCharacter::ABossCharacter()
{
	MaxHP = 30.f;
}

float ABossCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage <= 0.f || CurrentHP <= 0.f)
	{
		return ActualDamage;
	}

	const float HealthFraction = CurrentHP / MaxHP;

	EBossPhase NewPhase = EBossPhase::Phase1;
	if (HealthFraction <= Phase3HealthThreshold)
	{
		NewPhase = EBossPhase::Phase3;
	}
	else if (HealthFraction <= Phase2HealthThreshold)
	{
		NewPhase = EBossPhase::Phase2;
	}

	// Phases only ever advance, even if the boss is healed back above a threshold.
	if (NewPhase > CurrentPhase)
	{
		CurrentPhase = NewPhase;
		UE_LOG(LogWanderersDescent, Log, TEXT("%s entered phase %d"), *GetName(), static_cast<int32>(NewPhase) + 1);
		OnEnterPhase(NewPhase);
		OnBossPhaseChanged.Broadcast(NewPhase);
	}

	return ActualDamage;
}

void ABossCharacter::OnEnterPhase_Implementation(EBossPhase NewPhase)
{
}
