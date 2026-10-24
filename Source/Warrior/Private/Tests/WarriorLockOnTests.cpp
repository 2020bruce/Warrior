// Fill out your copyright notice in the Description page of Project Settings.

#include "Combat/LockOnTargeting.h"
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FWarriorLockOnTargetingTest,
	"Warrior.Combat.LockOn.Targeting",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FWarriorLockOnTargetingTest::RunTest(const FString& Parameters)
{
	const FVector Forward = FVector::ForwardVector;
	const FVector Right = FVector::RightVector;

	TestNearlyEqual(
		TEXT("Target directly to the right is 90 degrees"),
		WarriorLockOn::ComputeSignedAngleDegrees(Forward, Right, Right),
		90.0f,
		0.01f);
	TestNearlyEqual(
		TEXT("Target directly to the left is -90 degrees"),
		WarriorLockOn::ComputeSignedAngleDegrees(Forward, Right, -Right),
		-90.0f,
		0.01f);

	TestEqual(
		TEXT("Right switch advances to the next candidate"),
		WarriorLockOn::GetAdjacentIndex(1, 4, true),
		2);
	TestEqual(
		TEXT("Left switch advances to the previous candidate"),
		WarriorLockOn::GetAdjacentIndex(1, 4, false),
		0);
	TestEqual(
		TEXT("Right switch wraps from the last target to the first"),
		WarriorLockOn::GetAdjacentIndex(3, 4, true),
		0);
	TestEqual(
		TEXT("Left switch wraps from the first target to the last"),
		WarriorLockOn::GetAdjacentIndex(0, 4, false),
		3);
	TestEqual(
		TEXT("Switch rejects an empty candidate list"),
		WarriorLockOn::GetAdjacentIndex(0, 0, true),
		INDEX_NONE);

	const float CenterScore = WarriorLockOn::ComputeCandidateScore(0.0f, 1200.0f, 50.0f, 1800.0f, 0.7f, 0.3f);
	const float OffCenterScore = WarriorLockOn::ComputeCandidateScore(40.0f, 600.0f, 50.0f, 1800.0f, 0.7f, 0.3f);
	TestTrue(TEXT("Camera-center target wins even when a closer target is off-center"), CenterScore < OffCenterScore);

	return true;
}

#endif
