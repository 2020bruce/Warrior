// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "WarriorLockOnTrainingTarget.generated.h"

class UCapsuleComponent;
class UStaticMeshComponent;
class UTextRenderComponent;

/**
 * Visible, non-hostile pawn used to exercise lock-on when the scene has no enemies.
 */
UCLASS()
class WARRIOR_API AWarriorLockOnTrainingTarget : public APawn
{
	GENERATED_BODY()

public:
	AWarriorLockOnTrainingTarget();

private:
	UPROPERTY(VisibleAnywhere, Category = "Training Target")
	TObjectPtr<UCapsuleComponent> CollisionComponent;

	UPROPERTY(VisibleAnywhere, Category = "Training Target")
	TObjectPtr<UStaticMeshComponent> BodyMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Training Target")
	TObjectPtr<UStaticMeshComponent> HeadMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Training Target")
	TObjectPtr<UTextRenderComponent> LabelComponent;
};
