#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, NewHealth, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

// Shared health/damage handling for the player and the boss. Dodge i-frames and other
// invincibility windows are expressed by toggling bIsInvincible from outside this component.
UCLASS(ClassGroup=(Combat), meta=(BlueprintSpawnableComponent))
class WANDERERSDESCENT_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Health")
	float MaxHealth = 100.f;

	UPROPERTY(BlueprintReadOnly, Category="Health")
	float CurrentHealth = 0.f;

	UPROPERTY(BlueprintReadOnly, Category="Health")
	bool bIsInvincible = false;

	UPROPERTY(BlueprintAssignable, Category="Health")
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="Health")
	FOnDeath OnDeath;

	// Returns the amount of damage actually applied (0 if invincible, dead, or non-positive).
	UFUNCTION(BlueprintCallable, Category="Health")
	float ApplyDamage(float DamageAmount);

	UFUNCTION(BlueprintCallable, Category="Health")
	void Heal(float HealAmount);

	UFUNCTION(BlueprintPure, Category="Health")
	bool IsDead() const { return CurrentHealth <= 0.f; }

protected:
	virtual void BeginPlay() override;
};
