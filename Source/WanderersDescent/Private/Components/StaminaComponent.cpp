#include "Components/StaminaComponent.h"

UStaminaComponent::UStaminaComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentStamina = MaxStamina;
}

bool UStaminaComponent::TrySpend(float Cost)
{
	if (!CanAfford(Cost))
	{
		return false;
	}

	CurrentStamina -= Cost;
	TimeSinceLastSpend = 0.f;
	OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);
	return true;
}

void UStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TimeSinceLastSpend += DeltaTime;
	if (TimeSinceLastSpend < RegenDelay || CurrentStamina >= MaxStamina)
	{
		return;
	}

	CurrentStamina = FMath::Min(CurrentStamina + RegenPerSecond * DeltaTime, MaxStamina);
	OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);
}
