#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BossCharacter.generated.h"

class UHealthComponent;

UENUM(BlueprintType)
enum class EBossPhase : uint8
{
	Phase1,
	Phase2,
	Phase3
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBossPhaseChanged, EBossPhase, NewPhase);

// Multi-phase boss stub for the Soul Valley fight. Watches its own HealthComponent and
// advances phases at configurable health-percentage thresholds. Phase-specific attack
// patterns/animations belong in a Blueprint or C++ subclass overriding OnEnterPhase.
UCLASS()
class WANDERERSDESCENT_API ABossCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABossCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat")
	TObjectPtr<UHealthComponent> HealthComponent;

	UPROPERTY(BlueprintReadOnly, Category="Boss")
	EBossPhase CurrentPhase = EBossPhase::Phase1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Boss")
	float Phase2HealthThreshold = 0.66f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Boss")
	float Phase3HealthThreshold = 0.33f;

	UPROPERTY(BlueprintAssignable, Category="Boss")
	FOnBossPhaseChanged OnBossPhaseChanged;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleHealthChanged(float NewHealth, float MaxHealth);

	UFUNCTION(BlueprintNativeEvent, Category="Boss")
	void OnEnterPhase(EBossPhase NewPhase);
	virtual void OnEnterPhase_Implementation(EBossPhase NewPhase);
};
