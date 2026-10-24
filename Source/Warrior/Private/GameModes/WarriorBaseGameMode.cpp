// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/WarriorBaseGameMode.h"

#include "Characters/WarriorHeroCharacter.h"
#include "Combat/WarriorLockOnTrainingTarget.h"
#include "Controllers/WarriorHeroController.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "HAL/PlatformMisc.h"
#include "Misc/CommandLine.h"
#include "TimerManager.h"
#include "UI/WarriorControlsHUD.h"

DEFINE_LOG_CATEGORY_STATIC(LogWarriorTrainingTargets, Log, All);

AWarriorBaseGameMode::AWarriorBaseGameMode()
{
	DefaultPawnClass = AWarriorHeroCharacter::StaticClass();
	PlayerControllerClass = AWarriorHeroController::StaticClass();
	HUDClass = AWarriorControlsHUD::StaticClass();
	LockOnTrainingTargetClass = AWarriorLockOnTrainingTarget::StaticClass();
}

void AWarriorBaseGameMode::StartPlay()
{
	Super::StartPlay();

	if (bSpawnLockOnTrainingTargetsWhenSceneHasNoPawns && LockOnTrainingTargetClass)
	{
		GetWorldTimerManager().SetTimer(
			LockOnTrainingTargetsTimerHandle,
			this,
			&ThisClass::SpawnLockOnTrainingTargets,
			FMath::Max(0.0f, LockOnTrainingTargetSpawnDelay),
			false);
	}
}

void AWarriorBaseGameMode::SpawnLockOnTrainingTargets()
{
	UWorld* World = GetWorld();
	APlayerController* PlayerController = World ? World->GetFirstPlayerController() : nullptr;
	APawn* PlayerPawn = PlayerController ? PlayerController->GetPawn() : nullptr;
	if (!World || !PlayerController || !PlayerPawn)
	{
		UE_LOG(LogWarriorTrainingTargets, Warning, TEXT("Skipped lock-on training targets because no player pawn is available."));
		return;
	}

	int32 ExistingPawnCount = 0;
	for (TActorIterator<APawn> It(World); It; ++It)
	{
		const APawn* Candidate = *It;
		if (IsValid(Candidate) && Candidate != PlayerPawn)
		{
			++ExistingPawnCount;
		}
	}

	if (ExistingPawnCount > 0)
	{
		UE_LOG(
			LogWarriorTrainingTargets,
			Log,
			TEXT("Skipped lock-on training targets because the scene already contains %d other pawn(s)."),
			ExistingPawnCount);
		return;
	}

	const int32 TargetCount = FMath::Max(1, LockOnTrainingTargetCount);
	const FVector PlayerLocation = PlayerPawn->GetActorLocation();
	const FRotator ViewYaw(0.0f, PlayerController->GetControlRotation().Yaw, 0.0f);
	const FVector Forward = FRotationMatrix(ViewYaw).GetUnitAxis(EAxis::X);
	const FVector Right = FRotationMatrix(ViewYaw).GetUnitAxis(EAxis::Y);
	int32 SpawnedCount = 0;

	for (int32 TargetIndex = 0; TargetIndex < TargetCount; ++TargetIndex)
	{
		const float LateralOffset =
			(static_cast<float>(TargetIndex) - (static_cast<float>(TargetCount) - 1.0f) * 0.5f)
			* LockOnTrainingTargetSpacing;
		const FVector SpawnLocation =
			PlayerLocation
			+ Forward * LockOnTrainingTargetForwardDistance
			+ Right * LateralOffset;
		const FRotator SpawnRotation = (PlayerLocation - SpawnLocation).Rotation();

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;
		SpawnParameters.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		APawn* SpawnedTarget = World->SpawnActor<APawn>(
			LockOnTrainingTargetClass,
			SpawnLocation,
			SpawnRotation,
			SpawnParameters);
		if (SpawnedTarget)
		{
			++SpawnedCount;
			UE_LOG(
				LogWarriorTrainingTargets,
				Log,
				TEXT("Spawned lock-on training target %s at %s."),
				*SpawnedTarget->GetName(),
				*SpawnLocation.ToCompactString());
		}
	}

	if (SpawnedCount > 0)
	{
		UE_LOG(
			LogWarriorTrainingTargets,
			Log,
			TEXT("Lock-on acceptance setup ready with %d selectable target(s)."),
			SpawnedCount);
	}

#if WITH_DEV_AUTOMATION_TESTS
	if (FParse::Param(FCommandLine::Get(), TEXT("WarriorLockOnSmokeTest")))
	{
		AWarriorHeroController* WarriorController = Cast<AWarriorHeroController>(PlayerController);
		FString FailureReason;
		const bool bPassed =
			SpawnedCount > 0
			&& WarriorController
			&& WarriorController->RunLockOnPieSmokeTest(FailureReason);

		if (!bPassed)
		{
			UE_LOG(
				LogWarriorTrainingTargets,
				Error,
				TEXT("LOCKON_PIE_SMOKE_TEST FAIL: %s"),
				FailureReason.IsEmpty() ? TEXT("training targets or player controller unavailable") : *FailureReason);
		}

		FPlatformMisc::RequestExitWithStatus(false, bPassed ? 0 : 1);
	}
#endif
}
