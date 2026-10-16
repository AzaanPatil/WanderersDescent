#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StaminaComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStaminaChanged, float, NewStamina, float, MaxStamina);

// Governs how aggressively the player can act: attacks and dodges spend stamina,
// which regenerates after a short delay once spending stops.
UCLASS(ClassGroup=(Combat), meta=(BlueprintSpawnableComponent))
class WANDERERSDESCENT_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStaminaComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Stamina")
	float MaxStamina = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Stamina")
	float RegenPerSecond = 15.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Stamina")
	float RegenDelay = 0.75f;

	UPROPERTY(BlueprintReadOnly, Category="Stamina")
	float CurrentStamina = 0.f;

	UPROPERTY(BlueprintAssignable, Category="Stamina")
	FOnStaminaChanged OnStaminaChanged;

	UFUNCTION(BlueprintPure, Category="Stamina")
	bool CanAfford(float Cost) const { return CurrentStamina >= Cost; }

	// Returns true if there was enough stamina to spend.
	UFUNCTION(BlueprintCallable, Category="Stamina")
	bool TrySpend(float Cost);

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	float TimeSinceLastSpend = 0.f;
};
