#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PowerUpComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPowerUpActivated, FName, PowerUpName);

/**
 *  Combat power-ups bound to an array of keys, sharing one cooldown between activations.
 *  What each power-up actually does is implemented by whoever binds OnPowerUpActivated
 *  (e.g. the character Blueprint), so this component stays agnostic of the effects.
 */
UCLASS(ClassGroup=(Combat), meta=(BlueprintSpawnableComponent))
class WANDERERSDESCENT_API UPowerUpComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPowerUpComponent();

	/** One entry per key, matched by index to the character's PowerUpActions. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PowerUp")
	TArray<FName> PowerUpSlots;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PowerUp", meta=(ClampMin=0, Units="s"))
	float CooldownDuration = 10.f;

	UPROPERTY(BlueprintAssignable, Category="PowerUp")
	FOnPowerUpActivated OnPowerUpActivated;

	UFUNCTION(BlueprintCallable, Category="PowerUp")
	bool TryActivate(int32 SlotIndex);

	UFUNCTION(BlueprintPure, Category="PowerUp")
	bool IsOnCooldown() const;

	UFUNCTION(BlueprintPure, Category="PowerUp")
	float GetCooldownRemaining() const;

private:
	FTimerHandle CooldownTimer;
};
