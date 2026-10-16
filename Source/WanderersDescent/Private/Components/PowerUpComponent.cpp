#include "Components/PowerUpComponent.h"
#include "TimerManager.h"

UPowerUpComponent::UPowerUpComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UPowerUpComponent::TryActivate(int32 SlotIndex)
{
	if (bIsOnCooldown || !PowerUpSlots.IsValidIndex(SlotIndex))
	{
		return false;
	}

	OnPowerUpActivated.Broadcast(PowerUpSlots[SlotIndex]);

	bIsOnCooldown = true;
	GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &UPowerUpComponent::EndCooldown, CooldownDuration, false);
	return true;
}

void UPowerUpComponent::EndCooldown()
{
	bIsOnCooldown = false;
}
