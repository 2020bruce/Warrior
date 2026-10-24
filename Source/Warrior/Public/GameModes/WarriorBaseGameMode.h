// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WarriorBaseGameMode.generated.h"

class APawn;

/**
 *
 */
UCLASS(Config = Game)
class WARRIOR_API AWarriorBaseGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AWarriorBaseGameMode();

protected:
	virtual void StartPlay() override;

	UPROPERTY(EditDefaultsOnly, Config, Category = "Lock-On|Training")
	bool bSpawnLockOnTrainingTargetsWhenSceneHasNoPawns = true;

	UPROPERTY(EditDefaultsOnly, Config, Category = "Lock-On|Training")
	TSubclassOf<APawn> LockOnTrainingTargetClass;

	UPROPERTY(EditDefaultsOnly, Config, Category = "Lock-On|Training", meta = (ClampMin = "0.0"))
	float LockOnTrainingTargetSpawnDelay = 0.25f;

	UPROPERTY(EditDefaultsOnly, Config, Category = "Lock-On|Training", meta = (ClampMin = "1"))
	int32 LockOnTrainingTargetCount = 2;

	UPROPERTY(EditDefaultsOnly, Config, Category = "Lock-On|Training", meta = (ClampMin = "100.0"))
	float LockOnTrainingTargetForwardDistance = 1000.0f;

	UPROPERTY(EditDefaultsOnly, Config, Category = "Lock-On|Training", meta = (ClampMin = "0.0"))
	float LockOnTrainingTargetSpacing = 1200.0f;

private:
	void SpawnLockOnTrainingTargets();

	FTimerHandle LockOnTrainingTargetsTimerHandle;
};
