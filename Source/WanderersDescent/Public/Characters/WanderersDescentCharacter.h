#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "WanderersDescentCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UStaminaComponent;
class UCombatComponent;
class UPowerUpComponent;
class UHealthComponent;
class UInputMappingContext;
class UInputAction;

// Third-person player character. Movement/camera are set up here; combat, stamina and
// power-up behavior are delegated to their respective components. Input assets (Input
// Actions and the Input Mapping Context) are data assets created in the editor and
// assigned on a Blueprint subclass of this class - see README.md.
UCLASS()
class WANDERERSDESCENT_API AWanderersDescentCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AWanderersDescentCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta=(AllowPrivateAccess="true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UStaminaComponent> StaminaComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UCombatComponent> CombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UPowerUpComponent> PowerUpComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UHealthComponent> HealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> LightAttackAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> HeavyAttackAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> DodgeAction;

	// One entry per power-up slot, matched by index to UPowerUpComponent::PowerUpSlots.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TArray<TObjectPtr<UInputAction>> PowerUpActions;

private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void LightAttack(const FInputActionValue& Value);
	void HeavyAttack(const FInputActionValue& Value);
	void Dodge(const FInputActionValue& Value);
	void ActivatePowerUp(int32 SlotIndex);
	void DebugInput(const FString& Message) const;
};
