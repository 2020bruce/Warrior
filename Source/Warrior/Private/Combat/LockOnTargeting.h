#pragma once

#include "CoreMinimal.h"

namespace WarriorLockOn
{
	inline float ComputeSignedAngleDegrees(const FVector& ViewForward, const FVector& ViewRight, const FVector& DirectionToTarget)
	{
		const FVector SafeDirection = DirectionToTarget.GetSafeNormal();
		if (SafeDirection.IsNearlyZero())
		{
			return 0.0f;
		}

		return FMath::RadiansToDegrees(
			FMath::Atan2(FVector::DotProduct(SafeDirection, ViewRight), FVector::DotProduct(SafeDirection, ViewForward)));
	}

	inline float ComputeCandidateScore(
		float AbsoluteAngleDegrees,
		float Distance,
		float MaxAngleDegrees,
		float MaxDistance,
		float AngleWeight,
		float DistanceWeight)
	{
		const float NormalizedAngle = MaxAngleDegrees > UE_SMALL_NUMBER
			? FMath::Clamp(AbsoluteAngleDegrees / MaxAngleDegrees, 0.0f, 1.0f)
			: 1.0f;
		const float NormalizedDistance = MaxDistance > UE_SMALL_NUMBER
			? FMath::Clamp(Distance / MaxDistance, 0.0f, 1.0f)
			: 1.0f;

		return (NormalizedAngle * AngleWeight) + (NormalizedDistance * DistanceWeight);
	}

	inline int32 GetAdjacentIndex(int32 CurrentIndex, int32 CandidateCount, bool bSwitchRight)
	{
		if (CurrentIndex < 0 || CurrentIndex >= CandidateCount)
		{
			return INDEX_NONE;
		}

		return bSwitchRight
			? (CurrentIndex + 1) % CandidateCount
			: (CurrentIndex + CandidateCount - 1) % CandidateCount;
	}
}
