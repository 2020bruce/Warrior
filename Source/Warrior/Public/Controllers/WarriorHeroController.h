// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputCoreTypes.h"
#include "WarriorHeroController.generated.h"

class UInputAction;
class UInputMappingContext;

/**
 * Owns lock-on target selection and the locked camera rotation.
 */
UCLASS(Config = Game)
class WARRIOR_API AWarriorHeroController : public APlayerController
{
	GENERATED_BODY()

public:
	AWarriorHeroController();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

	UFUNCTION(BlueprintCallable, Category = "Lock-On")
	void ToggleLockOn();

	UFUNCTION(BlueprintCallable, Category = "Lock-On")
	void SwitchLockOnTarget(bool bSwitchRight);

	UFUNCTION(BlueprintCallable, Category = "Lock-On")
	void SetLockOnTarget(AActor* NewTarget);

	UFUNCTION(BlueprintCallable, Category = "Lock-On")
	void ClearLockOnTarget();

	UFUNCTION(BlueprintPure, Category = "Lock-On")
	AActor* GetLockOnTarget() const;

	UFUNCTION(BlueprintPure, Category = "Lock-On")
	bool IsLockedOn() const;

#if WITH_DEV_AUTOMATION_TESTS
	bool RunLockOnPieSmokeTest(FString& OutFailureReason);
#endif

protected:
	UPROPERTY(EditDefaultsOnly, Config, Category = "Lock-On|Targeting", meta = (ClampMin = "100.0"))
	float MaxLockOnDistance = 1800.0f;

	UPROPERTY(EditDefaultsOnly, Config, Category = "Lock-On|Targeting", meta = (ClampMin = "1.0"))
	float TargetBreakDistanceMultiplier = 1.15f;

	UPROPERTY(EditDefaultsOnly, Config, Category = "Lock-On|Targeting", meta = (ClampMin = "1.0", ClampMax = "89.0"))
	float MaxLockOnAcquireAngle = 50.0f;

	UPROPERTY(EditDefaultsOnly, Config, Category = "Lock-On|Targeting", meta = (ClampMin = "1.0", ClampMax = "179.0"))
	float MaxLockOnSwitchAngle = 85.0f;

	UPROPERTY(EditDefaultsOnly, Config, Category = "Lock-On|Targeting", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float AngleScoreWeight = 0.7f;

	UPROPERTY(EditDefaultsOnly, Config, Category = "Lock-On|Targeting", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float DistanceScoreWeight = 0.3f;

	UPROPERTY(EditDefaultsOnly, Config, Category = "Lock-On|Targeting", meta = (ClampMin = "0.0"))
	float LineOfSightBreakGrace = 0.35f;

	UPROPERTY(EditDefaultsOnly, Config, Category = "Lock-On|Targeting")
	FName RequiredTargetTag;

	UPROPERTY(EditDefaultsOnly, Config, Category = "Lock-On|Targeting")
	FName IgnoredTargetTag = TEXT("LockOnIgnored");

	UPROPERTY(EditDefaultsOnly, Config, Category = "Lock-On|Camera", meta = (ClampMin = "0.1"))
	float LockOnRotationInterpSpeed = 12.0f;

	UPROPERTY(EditDefaultsOnly, Config, Category = "Lock-On|Camera")
	float MinLockOnPitch = -70.0f;

	UPROPERTY(EditDefaultsOnly, Config, Category = "Lock-On|Camera")
	float MaxLockOnPitch = 70.0f;

	UPROPERTY(EditDefaultsOnly, Config, Category = "Lock-On|Camera")
	float TargetFocusHeightOffset = 20.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Lock-On|Input")
	TArray<FKey> LockOnToggleKeys;

	UPROPERTY(EditDefaultsOnly, Category = "Lock-On|Input")
	TArray<FKey> PreviousTargetKeys;

	UPROPERTY(EditDefaultsOnly, Category = "Lock-On|Input")
	TArray<FKey> NextTargetKeys;

	UPROPERTY(EditDefaultsOnly, Config, Category = "Lock-On|Input")
	int32 LockOnInputPriority = 10;

private:
	void EnsureRuntimeInputMappings();
	void ApplyInputMappingContext();
	void RemoveInputMappingContext();

	void HandleToggleLockOn();
	void HandlePreviousTarget();
	void HandleNextTarget();

	bool AcquireBestLockOnTarget();
	bool GetLockOnView(FVector& OutViewLocation, FVector& OutViewForward) const;
	bool IsLockOnTargetValid(AActor* Candidate, float MaxDistance, float MaxAngleDegrees) const;
	bool HasLockOnLineOfSight(AActor* Candidate) const;
	float GetCandidateScore(AActor* Candidate) const;
	float GetSignedTargetAngle(AActor* Candidate, const FVector& ViewForward) const;
	void CollectLockOnCandidates(TArray<AActor*>& OutCandidates, float MaxDistance, float MaxAngleDegrees) const;
	void UpdateLockedCamera(float DeltaTime);
	FVector GetTargetFocusLocation(AActor* Target) const;
	void AssignLockOnTarget(AActor* NewTarget);
	void LogTargetChange(AActor* PreviousTarget, AActor* NewTarget) const;

	UPROPERTY(Transient)
	TObjectPtr<UInputAction> LockOnToggleAction;

	UPROPERTY(Transient)
	TObjectPtr<UInputAction> PreviousTargetAction;

	UPROPERTY(Transient)
	TObjectPtr<UInputAction> NextTargetAction;

	UPROPERTY(Transient)
	TObjectPtr<UInputMappingContext> LockOnMappingContext;

	TWeakObjectPtr<AActor> CurrentLockOnTarget;
	float TimeSinceTargetOccluded = 0.0f;
	bool bLockOnInputBound = false;
	bool bLockOnMappingContextApplied = false;
};
