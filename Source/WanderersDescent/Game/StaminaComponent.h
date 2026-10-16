#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StaminaComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStaminaChanged, float, NewStamina, float, MaxStamina);

/**
 *  Governs how aggressively the player can act: attacks, dodges and sprinting spend stamina,
 *  which regenerates after a short delay once spending stops.
 */
UCLASS(ClassGroup=(Combat), meta=(BlueprintSpawnableComponent))
class WANDERERSDESCENT_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStaminaComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stamina", meta=(ClampMin=1))
	float MaxStamina = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stamina", meta=(ClampMin=0))
	float RegenPerSecond = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stamina", meta=(ClampMin=0, Units="s"))
	float RegenDelay = 0.75f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Stamina")
	float CurrentStamina = 0.f;

	UPROPERTY(BlueprintAssignable, Category="Stamina")
	FOnStaminaChanged OnStaminaChanged;

	UFUNCTION(BlueprintPure, Category="Stamina")
	bool CanAfford(float Cost) const { return CurrentStamina >= Cost; }

	/** Spends the stamina and returns true, or returns false and spends nothing if there isn't enough. */
	UFUNCTION(BlueprintCallable, Category="Stamina")
	bool TrySpend(float Cost);

	UFUNCTION(BlueprintPure, Category="Stamina")
	float GetStaminaPercent() const { return CurrentStamina / MaxStamina; }

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	float TimeSinceLastSpend = 0.f;
};
