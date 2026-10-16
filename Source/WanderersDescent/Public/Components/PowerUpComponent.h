#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PowerUpComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPowerUpActivated, FName, PowerUpName);

// Combat power-ups bound to an array of keys, sharing one global cooldown between activations.
// The actual gameplay effect of each power-up is left to a Blueprint/C++ handler bound to
// OnPowerUpActivated, keeping this component agnostic of what any given power-up does.
UCLASS(ClassGroup=(Combat), meta=(BlueprintSpawnableComponent))
class WANDERERSDESCENT_API UPowerUpComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPowerUpComponent();

	// One entry per key/slot; assign identifiers in the character Blueprint or Details panel.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="PowerUp")
	TArray<FName> PowerUpSlots;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="PowerUp")
	float CooldownDuration = 10.f;

	UPROPERTY(BlueprintReadOnly, Category="PowerUp")
	bool bIsOnCooldown = false;

	UPROPERTY(BlueprintAssignable, Category="PowerUp")
	FOnPowerUpActivated OnPowerUpActivated;

	UFUNCTION(BlueprintCallable, Category="PowerUp")
	bool TryActivate(int32 SlotIndex);

private:
	FTimerHandle CooldownTimerHandle;

	void EndCooldown();
};
