#include "Characters/BossCharacter.h"
#include "Components/HealthComponent.h"

ABossCharacter::ABossCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

void ABossCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HealthComponent)
	{
		HealthComponent->OnHealthChanged.AddDynamic(this, &ABossCharacter::HandleHealthChanged);
	}
}

void ABossCharacter::HandleHealthChanged(float NewHealth, float MaxHealth)
{
	if (MaxHealth <= 0.f)
	{
		return;
	}

	const float HealthPct = NewHealth / MaxHealth;

	EBossPhase NewPhase = CurrentPhase;
	if (HealthPct <= Phase3HealthThreshold)
	{
		NewPhase = EBossPhase::Phase3;
	}
	else if (HealthPct <= Phase2HealthThreshold)
	{
		NewPhase = EBossPhase::Phase2;
	}

	if (NewPhase != CurrentPhase)
	{
		CurrentPhase = NewPhase;
		OnEnterPhase(NewPhase);
		OnBossPhaseChanged.Broadcast(NewPhase);
	}
}

void ABossCharacter::OnEnterPhase_Implementation(EBossPhase NewPhase)
{
}
