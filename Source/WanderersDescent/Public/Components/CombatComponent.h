#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

class UStaminaComponent;
class UHealthComponent;

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	Idle,
	LightAttack,
	HeavyAttack,
	Dodging
};

// Light/heavy attacks and a dodge with brief invincibility frames, all gated by stamina.
// Looks up sibling UStaminaComponent / UHealthComponent on the owning actor at BeginPlay.
UCLASS(ClassGroup=(Combat), meta=(BlueprintSpawnableComponent))
class WANDERERSDESCENT_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat|Cost")
	float LightAttackStaminaCost = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat|Cost")
	float HeavyAttackStaminaCost = 25.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat|Cost")
	float DodgeStaminaCost = 15.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat|Timing")
	float LightAttackDuration = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat|Timing")
	float HeavyAttackDuration = 0.9f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat|Timing")
	float DodgeDuration = 0.5f;

	// Portion of DodgeDuration, starting at dodge start, during which the owner is invincible.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat|Timing")
	float DodgeInvincibleDuration = 0.25f;

	UPROPERTY(BlueprintReadOnly, Category="Combat")
	ECombatState CombatState = ECombatState::Idle;

	UFUNCTION(BlueprintCallable, Category="Combat")
	bool TryLightAttack();

	UFUNCTION(BlueprintCallable, Category="Combat")
	bool TryHeavyAttack();

	UFUNCTION(BlueprintCallable, Category="Combat")
	bool TryDodge();

	UFUNCTION(BlueprintPure, Category="Combat")
	bool IsBusy() const { return CombatState != ECombatState::Idle; }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<UStaminaComponent> StaminaComponent;

	UPROPERTY()
	TObjectPtr<UHealthComponent> HealthComponent;

	FTimerHandle StateTimerHandle;
	FTimerHandle InvincibilityTimerHandle;

	void EnterState(ECombatState NewState, float Duration);
	void ReturnToIdle();
	void BeginDodgeInvincibility();
	void EndDodgeInvincibility();
};
