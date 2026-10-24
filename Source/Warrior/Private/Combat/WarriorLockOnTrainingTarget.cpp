// Fill out your copyright notice in the Description page of Project Settings.

#include "Combat/WarriorLockOnTrainingTarget.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"

AWarriorLockOnTrainingTarget::AWarriorLockOnTrainingTarget()
{
	PrimaryActorTick.bCanEverTick = false;
	AutoPossessAI = EAutoPossessAI::Disabled;

	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
	CollisionComponent->InitCapsuleSize(42.0f, 100.0f);
	CollisionComponent->SetCollisionProfileName(TEXT("BlockAll"));
	SetRootComponent(CollisionComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMesh(
		TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(
		TEXT("/Engine/BasicShapes/Sphere.Sphere"));

	BodyMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMeshComponent"));
	BodyMeshComponent->SetupAttachment(CollisionComponent);
	BodyMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -20.0f));
	BodyMeshComponent->SetRelativeScale3D(FVector(0.9f, 0.9f, 1.6f));
	BodyMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BodyMeshComponent->SetGenerateOverlapEvents(false);
	if (CylinderMesh.Succeeded())
	{
		BodyMeshComponent->SetStaticMesh(CylinderMesh.Object);
	}

	HeadMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeadMeshComponent"));
	HeadMeshComponent->SetupAttachment(CollisionComponent);
	HeadMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 82.0f));
	HeadMeshComponent->SetRelativeScale3D(FVector(0.65f));
	HeadMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HeadMeshComponent->SetGenerateOverlapEvents(false);
	if (SphereMesh.Succeeded())
	{
		HeadMeshComponent->SetStaticMesh(SphereMesh.Object);
	}

	LabelComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("LabelComponent"));
	LabelComponent->SetupAttachment(CollisionComponent);
	LabelComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 162.0f));
	LabelComponent->SetText(FText::FromString(TEXT("LOCK-ON TARGET")));
	LabelComponent->SetTextRenderColor(FColor(255, 164, 64));
	LabelComponent->SetWorldSize(34.0f);
	LabelComponent->SetHorizontalAlignment(EHTA_Center);
	LabelComponent->SetVerticalAlignment(EVRTA_TextCenter);
}
