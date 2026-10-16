#pragma once

#include "CoreMinimal.h"
#include "CombatEnemy.h"
#include "BossCharacter.generated.h"

UENUM(BlueprintType)
enum class EBossPhase : uint8
{
	Phase1,
	Phase2,
	Phase3
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBossPhaseChanged, EBossPhase, NewPhase);

/**
 *  The Soul Valley boss. Uses the template enemy's StateTree AI, attacks and life bar, and
 *  advances through phases as its health crosses the thresholds below. Phase-specific attacks
 *  and animations go in OnEnterPhase (override it in the Blueprint).
 */
UCLASS(abstract)
class WANDERERSDESCENT_API ABossCharacter : public ACombatEnemy
{
	GENERATED_BODY()

public:

	ABossCharacter();

	/** Fraction of max HP (0-1) at or below which phase 2 starts. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Boss", meta=(ClampMin=0, ClampMax=1))
	float Phase2HealthThreshold = 0.66f;

	/** Fraction of max HP (0-1) at or below which phase 3 starts. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Boss", meta=(ClampMin=0, ClampMax=1))
	float Phase3HealthThreshold = 0.33f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Boss")
	EBossPhase CurrentPhase = EBossPhase::Phase1;

	UPROPERTY(BlueprintAssignable, Category="Boss")
	FOnBossPhaseChanged OnBossPhaseChanged;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:

	UFUNCTION(BlueprintNativeEvent, Category="Boss")
	void OnEnterPhase(EBossPhase NewPhase);
	virtual void OnEnterPhase_Implementation(EBossPhase NewPhase);
};
