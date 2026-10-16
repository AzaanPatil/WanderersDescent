#include "PowerUpComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"

UPowerUpComponent::UPowerUpComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UPowerUpComponent::TryActivate(int32 SlotIndex)
{
	if (IsOnCooldown() || !PowerUpSlots.IsValidIndex(SlotIndex))
	{
		return false;
	}

	// The timer only needs to exist to be queried; nothing runs when it expires.
	GetWorld()->GetTimerManager().SetTimer(CooldownTimer, CooldownDuration, false);

	OnPowerUpActivated.Broadcast(PowerUpSlots[SlotIndex]);
	return true;
}

bool UPowerUpComponent::IsOnCooldown() const
{
	return GetWorld()->GetTimerManager().IsTimerActive(CooldownTimer);
}

float UPowerUpComponent::GetCooldownRemaining() const
{
	return FMath::Max(0.f, GetWorld()->GetTimerManager().GetTimerRemaining(CooldownTimer));
}
