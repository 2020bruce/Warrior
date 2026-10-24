// Fill out your copyright notice in the Description page of Project Settings.

#include "Controllers/WarriorHeroController.h"

#include "Algo/Sort.h"
#include "Camera/PlayerCameraManager.h"
#include "CollisionQueryParams.h"
#include "Combat/LockOnTargeting.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Pawn.h"
#include "InputAction.h"
#include "InputMappingContext.h"

DEFINE_LOG_CATEGORY_STATIC(LogWarriorLockOn, Log, All);

AWarriorHeroController::AWarriorHeroController()
{
	PrimaryActorTick.bCanEverTick = true;

	LockOnToggleKeys = { EKeys::MiddleMouseButton, EKeys::Gamepad_RightThumbstick };
	PreviousTargetKeys = { EKeys::MouseScrollUp, EKeys::Q, EKeys::Gamepad_DPad_Left };
	NextTargetKeys = { EKeys::MouseScrollDown, EKeys::E, EKeys::Gamepad_DPad_Right };
}

void AWarriorHeroController::BeginPlay()
{
	Super::BeginPlay();

	EnsureRuntimeInputMappings();
	ApplyInputMappingContext();
}

void AWarriorHeroController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	RemoveInputMappingContext();
	Super::EndPlay(EndPlayReason);
}

void AWarriorHeroController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (!IsLocalController())
	{
		return;
	}

	AActor* Target = CurrentLockOnTarget.Get();
	if (!IsValid(Target) || Target->IsActorBeingDestroyed() || Target->IsHidden())
	{
		ClearLockOnTarget();
		return;
	}

	const APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn)
	{
		ClearLockOnTarget();
		return;
	}

	const float BreakDistance = MaxLockOnDistance * FMath::Max(1.0f, TargetBreakDistanceMultiplier);
	if (FVector::DistSquared(ControlledPawn->GetActorLocation(), Target->GetActorLocation()) > FMath::Square(BreakDistance))
	{
		ClearLockOnTarget();
		return;
	}

	if (HasLockOnLineOfSight(Target))
	{
		TimeSinceTargetOccluded = 0.0f;
	}
	else
	{
		TimeSinceTargetOccluded += DeltaTime;
		if (TimeSinceTargetOccluded >= LineOfSightBreakGrace)
		{
			ClearLockOnTarget();
			return;
		}
	}

	UpdateLockedCamera(DeltaTime);
}

void AWarriorHeroController::SetupInputComponent()
{
	Super::SetupInputComponent();

	EnsureRuntimeInputMappings();
	ApplyInputMappingContext();

	if (bLockOnInputBound)
	{
		return;
	}

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (!ensureMsgf(EnhancedInputComponent, TEXT("Warrior lock-on requires an Enhanced Input component.")))
	{
		return;
	}

	EnhancedInputComponent->BindAction(LockOnToggleAction, ETriggerEvent::Started, this, &ThisClass::HandleToggleLockOn);
	EnhancedInputComponent->BindAction(PreviousTargetAction, ETriggerEvent::Started, this, &ThisClass::HandlePreviousTarget);
	EnhancedInputComponent->BindAction(NextTargetAction, ETriggerEvent::Started, this, &ThisClass::HandleNextTarget);
	bLockOnInputBound = true;
}

void AWarriorHeroController::ToggleLockOn()
{
	if (IsLockedOn())
	{
		ClearLockOnTarget();
		return;
	}

	AcquireBestLockOnTarget();
}

void AWarriorHeroController::SwitchLockOnTarget(bool bSwitchRight)
{
	AActor* CurrentTarget = CurrentLockOnTarget.Get();
	if (!CurrentTarget)
	{
		return;
	}

	TArray<AActor*> Candidates;
	CollectLockOnCandidates(Candidates, MaxLockOnDistance, MaxLockOnSwitchAngle);

	if (Candidates.IsEmpty())
	{
		return;
	}

	FVector ViewLocation;
	FVector ViewForward;
	if (!GetLockOnView(ViewLocation, ViewForward))
	{
		return;
	}

	Algo::Sort(Candidates, [this, &ViewForward](AActor* Left, AActor* Right)
	{
		const float LeftAngle = GetSignedTargetAngle(Left, ViewForward);
		const float RightAngle = GetSignedTargetAngle(Right, ViewForward);
		if (!FMath::IsNearlyEqual(LeftAngle, RightAngle))
		{
			return LeftAngle < RightAngle;
		}

		return Left->GetName().Compare(Right->GetName()) < 0;
	});

	int32 CurrentIndex = INDEX_NONE;
	for (int32 Index = 0; Index < Candidates.Num(); ++Index)
	{
		if (Candidates[Index] == CurrentTarget)
		{
			CurrentIndex = Index;
			break;
		}
	}

	if (CurrentIndex == INDEX_NONE)
	{
		return;
	}

	const int32 NextIndex = WarriorLockOn::GetAdjacentIndex(CurrentIndex, Candidates.Num(), bSwitchRight);
	if (Candidates.IsValidIndex(NextIndex))
	{
		AssignLockOnTarget(Candidates[NextIndex]);
	}
}

void AWarriorHeroController::SetLockOnTarget(AActor* NewTarget)
{
	if (!NewTarget)
	{
		ClearLockOnTarget();
		return;
	}

	if (!IsLockOnTargetValid(NewTarget, MaxLockOnDistance, MaxLockOnAcquireAngle))
	{
		return;
	}

	AssignLockOnTarget(NewTarget);
}

void AWarriorHeroController::ClearLockOnTarget()
{
	AActor* PreviousTarget = CurrentLockOnTarget.Get();
	CurrentLockOnTarget.Reset();
	TimeSinceTargetOccluded = 0.0f;
	if (PreviousTarget)
	{
		LogTargetChange(PreviousTarget, nullptr);
	}
}

AActor* AWarriorHeroController::GetLockOnTarget() const
{
	return CurrentLockOnTarget.Get();
}

bool AWarriorHeroController::IsLockedOn() const
{
	return CurrentLockOnTarget.IsValid();
}

void AWarriorHeroController::EnsureRuntimeInputMappings()
{
	if (!LockOnToggleAction)
	{
		LockOnToggleAction = NewObject<UInputAction>(this, TEXT("IA_WarriorLockOnToggle"), RF_Transient);
		LockOnToggleAction->ValueType = EInputActionValueType::Boolean;
	}

	if (!PreviousTargetAction)
	{
		PreviousTargetAction = NewObject<UInputAction>(this, TEXT("IA_WarriorLockOnPrevious"), RF_Transient);
		PreviousTargetAction->ValueType = EInputActionValueType::Boolean;
	}

	if (!NextTargetAction)
	{
		NextTargetAction = NewObject<UInputAction>(this, TEXT("IA_WarriorLockOnNext"), RF_Transient);
		NextTargetAction->ValueType = EInputActionValueType::Boolean;
	}

	if (LockOnMappingContext)
	{
		return;
	}

	LockOnMappingContext = NewObject<UInputMappingContext>(this, TEXT("IMC_WarriorLockOn"), RF_Transient);
	for (const FKey& Key : LockOnToggleKeys)
	{
		if (Key.IsValid())
		{
			LockOnMappingContext->MapKey(LockOnToggleAction, Key);
		}
	}

	for (const FKey& Key : PreviousTargetKeys)
	{
		if (Key.IsValid())
		{
			LockOnMappingContext->MapKey(PreviousTargetAction, Key);
		}
	}

	for (const FKey& Key : NextTargetKeys)
	{
		if (Key.IsValid())
		{
			LockOnMappingContext->MapKey(NextTargetAction, Key);
		}
	}
}

void AWarriorHeroController::ApplyInputMappingContext()
{
	if (bLockOnMappingContextApplied || !LockOnMappingContext)
	{
		return;
	}

	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	if (!InputSubsystem)
	{
		return;
	}

	InputSubsystem->AddMappingContext(LockOnMappingContext, LockOnInputPriority);
	bLockOnMappingContextApplied = true;
	UE_LOG(LogWarriorLockOn, Log, TEXT("Applied lock-on input mapping at priority %d."), LockOnInputPriority);
}

void AWarriorHeroController::RemoveInputMappingContext()
{
	if (!bLockOnMappingContextApplied || !LockOnMappingContext)
	{
		return;
	}

	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	if (InputSubsystem)
	{
		InputSubsystem->RemoveMappingContext(LockOnMappingContext);
	}

	bLockOnMappingContextApplied = false;
}

void AWarriorHeroController::HandleToggleLockOn()
{
	ToggleLockOn();
}

void AWarriorHeroController::HandlePreviousTarget()
{
	SwitchLockOnTarget(false);
}

void AWarriorHeroController::HandleNextTarget()
{
	SwitchLockOnTarget(true);
}

#if WITH_DEV_AUTOMATION_TESTS
bool AWarriorHeroController::RunLockOnPieSmokeTest(FString& OutFailureReason)
{
	ClearLockOnTarget();
	if (!AcquireBestLockOnTarget())
	{
		OutFailureReason = TEXT("no lock-on candidate was acquired");
		return false;
	}

	AActor* FirstTarget = GetLockOnTarget();
	if (!IsLockedOn() || !IsValid(FirstTarget))
	{
		OutFailureReason = TEXT("lock-on did not retain its acquired target");
		return false;
	}

	SwitchLockOnTarget(true);
	AActor* SecondTarget = GetLockOnTarget();
	if (!IsLockedOn() || !IsValid(SecondTarget) || SecondTarget == FirstTarget)
	{
		OutFailureReason = TEXT("target switch did not select a different valid target");
		return false;
	}

	UE_LOG(
		LogWarriorLockOn,
		Log,
		TEXT("LOCKON_PIE_SMOKE_TEST PASS: acquired=%s switchedTo=%s"),
		*FirstTarget->GetName(),
		*SecondTarget->GetName());
	ClearLockOnTarget();
	return true;
}
#endif

bool AWarriorHeroController::AcquireBestLockOnTarget()
{
	TArray<AActor*> Candidates;
	CollectLockOnCandidates(Candidates, MaxLockOnDistance, MaxLockOnAcquireAngle);

	AActor* BestCandidate = nullptr;
	float BestScore = TNumericLimits<float>::Max();
	for (AActor* Candidate : Candidates)
	{
		const float Score = GetCandidateScore(Candidate);
		if (Score < BestScore)
		{
			BestScore = Score;
			BestCandidate = Candidate;
		}
	}

	if (!BestCandidate)
	{
		return false;
	}

	AssignLockOnTarget(BestCandidate);
	return true;
}

bool AWarriorHeroController::GetLockOnView(FVector& OutViewLocation, FVector& OutViewForward) const
{
	if (PlayerCameraManager)
	{
		OutViewLocation = PlayerCameraManager->GetCameraLocation();
		OutViewForward = PlayerCameraManager->GetCameraRotation().Vector();
		return true;
	}

	const APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn)
	{
		return false;
	}

	FRotator ViewRotation;
	ControlledPawn->GetActorEyesViewPoint(OutViewLocation, ViewRotation);
	OutViewForward = ViewRotation.Vector();
	return true;
}

bool AWarriorHeroController::IsLockOnTargetValid(AActor* Candidate, float MaxDistance, float MaxAngleDegrees) const
{
	const APawn* ControlledPawn = GetPawn();
	if (!IsValid(Candidate) || !ControlledPawn || Candidate == ControlledPawn ||
		Candidate->IsActorBeingDestroyed() || Candidate->IsHidden())
	{
		return false;
	}

	if (!RequiredTargetTag.IsNone() && !Candidate->ActorHasTag(RequiredTargetTag))
	{
		return false;
	}

	if (!IgnoredTargetTag.IsNone() && Candidate->ActorHasTag(IgnoredTargetTag))
	{
		return false;
	}

	if (FVector::DistSquared(ControlledPawn->GetActorLocation(), Candidate->GetActorLocation()) >
		FMath::Square(MaxDistance))
	{
		return false;
	}

	FVector ViewLocation;
	FVector ViewForward;
	if (!GetLockOnView(ViewLocation, ViewForward))
	{
		return false;
	}

	const FVector ToTarget = (GetTargetFocusLocation(Candidate) - ViewLocation).GetSafeNormal();
	const float MinFacingDot = FMath::Cos(FMath::DegreesToRadians(MaxAngleDegrees));
	return !ToTarget.IsNearlyZero()
		&& FVector::DotProduct(ViewForward, ToTarget) >= MinFacingDot
		&& HasLockOnLineOfSight(Candidate);
}

bool AWarriorHeroController::HasLockOnLineOfSight(AActor* Candidate) const
{
	UWorld* World = GetWorld();
	FVector ViewLocation;
	FVector ViewForward;
	if (!World || !IsValid(Candidate) || !GetLockOnView(ViewLocation, ViewForward))
	{
		return false;
	}

	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(WarriorLockOnVisibility), false);
	QueryParams.AddIgnoredActor(GetPawn());
	QueryParams.AddIgnoredActor(Candidate);

	FHitResult HitResult;
	return !World->LineTraceSingleByChannel(
		HitResult,
		ViewLocation,
		GetTargetFocusLocation(Candidate),
		ECC_Visibility,
		QueryParams);
}

float AWarriorHeroController::GetCandidateScore(AActor* Candidate) const
{
	const APawn* ControlledPawn = GetPawn();
	FVector ViewLocation;
	FVector ViewForward;
	if (!ControlledPawn || !GetLockOnView(ViewLocation, ViewForward))
	{
		return TNumericLimits<float>::Max();
	}

	const FVector DirectionToTarget = (GetTargetFocusLocation(Candidate) - ViewLocation).GetSafeNormal();
	const float AngleDegrees = FMath::RadiansToDegrees(
		FMath::Acos(FMath::Clamp(FVector::DotProduct(ViewForward, DirectionToTarget), -1.0f, 1.0f)));
	const float Distance = FVector::Dist(ControlledPawn->GetActorLocation(), Candidate->GetActorLocation());
	return WarriorLockOn::ComputeCandidateScore(
		AngleDegrees,
		Distance,
		MaxLockOnAcquireAngle,
		MaxLockOnDistance,
		AngleScoreWeight,
		DistanceScoreWeight);
}

float AWarriorHeroController::GetSignedTargetAngle(AActor* Candidate, const FVector& ViewForward) const
{
	FVector ViewLocation;
	FVector IgnoredViewForward;
	if (!GetLockOnView(ViewLocation, IgnoredViewForward))
	{
		return 0.0f;
	}

	const FVector ViewRight = FVector::CrossProduct(FVector::UpVector, ViewForward).GetSafeNormal();
	return WarriorLockOn::ComputeSignedAngleDegrees(
		ViewForward,
		ViewRight,
		GetTargetFocusLocation(Candidate) - ViewLocation);
}

void AWarriorHeroController::CollectLockOnCandidates(
	TArray<AActor*>& OutCandidates,
	float MaxDistance,
	float MaxAngleDegrees) const
{
	OutCandidates.Reset();
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	for (TActorIterator<APawn> It(World); It; ++It)
	{
		APawn* Candidate = *It;
		if (IsLockOnTargetValid(Candidate, MaxDistance, MaxAngleDegrees))
		{
			OutCandidates.Add(Candidate);
		}
	}
}

void AWarriorHeroController::UpdateLockedCamera(float DeltaTime)
{
	AActor* Target = CurrentLockOnTarget.Get();
	FVector ViewLocation;
	FVector ViewForward;
	if (!IsValid(Target) || !GetLockOnView(ViewLocation, ViewForward) || !PlayerCameraManager)
	{
		return;
	}

	FRotator DesiredRotation = (GetTargetFocusLocation(Target) - ViewLocation).Rotation();
	DesiredRotation.Pitch = FMath::Clamp(DesiredRotation.Pitch, MinLockOnPitch, MaxLockOnPitch);
	DesiredRotation.Roll = 0.0f;

	const FRotator NewRotation = LockOnRotationInterpSpeed > 0.0f
		? FMath::RInterpTo(GetControlRotation(), DesiredRotation, DeltaTime, LockOnRotationInterpSpeed)
		: DesiredRotation;
	SetControlRotation(NewRotation);
}

FVector AWarriorHeroController::GetTargetFocusLocation(AActor* Target) const
{
	if (!IsValid(Target))
	{
		return FVector::ZeroVector;
	}

	FVector Origin;
	FVector Extent;
	Target->GetActorBounds(false, Origin, Extent);
	Origin.Z += TargetFocusHeightOffset;
	return Origin;
}

void AWarriorHeroController::AssignLockOnTarget(AActor* NewTarget)
{
	AActor* PreviousTarget = CurrentLockOnTarget.Get();
	if (PreviousTarget == NewTarget)
	{
		return;
	}

	CurrentLockOnTarget = NewTarget;
	TimeSinceTargetOccluded = 0.0f;
	LogTargetChange(PreviousTarget, NewTarget);
}

void AWarriorHeroController::LogTargetChange(AActor* PreviousTarget, AActor* NewTarget) const
{
	if (PreviousTarget == NewTarget)
	{
		return;
	}

	if (NewTarget)
	{
		UE_LOG(
			LogWarriorLockOn,
			Log,
			TEXT("Lock-on target changed: %s -> %s"),
			PreviousTarget ? *PreviousTarget->GetName() : TEXT("None"),
			*NewTarget->GetName());
	}
	else
	{
		UE_LOG(
			LogWarriorLockOn,
			Log,
			TEXT("Lock-on target cleared: %s"),
			PreviousTarget ? *PreviousTarget->GetName() : TEXT("None"));
	}
}
