#pragma once

#include "CoreMinimal.h"
#include "CombatCharacter.h"
#include "WandererCharacter.generated.h"

class UStaminaComponent;
class UPowerUpComponent;
class UInputAction;
class UAnimMontage;

/**
 *  The player character. Builds on the template's CombatCharacter (combo/charged attacks,
 *  hit detection, damage, death, respawn) and adds stamina costs, sprint, a dodge with
 *  invincibility frames, and key-bound power-ups.
 */
UCLASS(abstract)
class WANDERERSDESCENT_API AWandererCharacter : public ACombatCharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UStaminaComponent> Stamina;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UPowerUpComponent> PowerUps;

protected:

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> DodgeAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> SprintAction;

	/** Entry i activates PowerUps->PowerUpSlots[i]. */
	UPROPERTY(EditAnywhere, Category="Input")
	TArray<TObjectPtr<UInputAction>> PowerUpActions;

	/** Charged per hit, including each follow-up hit of the combo string. */
	UPROPERTY(EditAnywhere, Category="Stamina", meta=(ClampMin=0))
	float LightAttackStaminaCost = 10.f;

	UPROPERTY(EditAnywhere, Category="Stamina", meta=(ClampMin=0))
	float HeavyAttackStaminaCost = 25.f;

	UPROPERTY(EditAnywhere, Category="Stamina", meta=(ClampMin=0))
	float DodgeStaminaCost = 20.f;

	UPROPERTY(EditAnywhere, Category="Stamina", meta=(ClampMin=0))
	float SprintStaminaPerSecond = 15.f;

	UPROPERTY(EditAnywhere, Category="Movement", meta=(ClampMin=0, Units="cm/s"))
	float SprintSpeed = 650.f;

	UPROPERTY(EditAnywhere, Category="Dodge", meta=(ClampMin=0, Units="cm/s"))
	float DodgeSpeed = 1200.f;

	/** How long the dodge locks out attacking and dodging again. */
	UPROPERTY(EditAnywhere, Category="Dodge", meta=(ClampMin=0, Units="s"))
	float DodgeDuration = 0.5f;

	/** Starts with the dodge; incoming damage is ignored for this long. */
	UPROPERTY(EditAnywhere, Category="Dodge", meta=(ClampMin=0, Units="s"))
	float DodgeInvincibilityDuration = 0.3f;

	/** Optional. Without it the dodge is just the launch, with no animation. */
	UPROPERTY(EditAnywhere, Category="Dodge")
	TObjectPtr<UAnimMontage> DodgeMontage;

	/** Prints combat events and stamina on screen while there's no HUD yet. */
	UPROPERTY(EditAnywhere, Category="Debug")
	bool bShowDebugMessages = true;

public:

	AWandererCharacter();

	virtual void Tick(float DeltaSeconds) override;

	virtual void DoComboAttackStart() override;
	virtual void DoChargedAttackStart() override;
	virtual void CheckCombo() override;

	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoDodge();

	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoSprintStart();

	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoSprintEnd();

	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoActivatePowerUp(int32 SlotIndex);

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintPure, Category="Dodge")
	bool IsInvincible() const { return bIsInvincible; }

	UFUNCTION(BlueprintPure, Category="Movement")
	bool IsSprinting() const { return bIsSprinting; }

protected:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:

	/** Fresh attacks are charged here, since the template can also start them from a buffered input. */
	UFUNCTION()
	void HandleMontageStarted(UAnimMontage* Montage);

	void EndDodge();
	void EndInvincibility();
	void DebugMessage(const FString& Message) const;

	bool bIsSprinting = false;
	bool bIsDodging = false;
	bool bIsInvincible = false;
	float WalkSpeed = 0.f;

	FTimerHandle DodgeTimer;
	FTimerHandle InvincibilityTimer;
};
