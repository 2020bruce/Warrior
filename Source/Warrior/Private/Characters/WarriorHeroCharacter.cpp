// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/WarriorHeroCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/ConstructorHelpers.h"

#include "WarriorDebugHelper.h"

AWarriorHeroCharacter::AWarriorHeroCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(52.f, 72.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a spring arm component
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 285.0f; // The camera follows at this distance behind the character
	SpringArmComponent->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	SpringArmComponent->SocketOffset = FVector(0.0f, 40.0f, 30.0f); // Offset the camera above the character
	// Create a camera component
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);

	// Show the generated Doraemon mesh while retaining ACharacter movement and collision.
	DoraemonMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoraemonMeshComponent"));
	DoraemonMeshComponent->SetupAttachment(GetCapsuleComponent());
	DoraemonMeshComponent->SetMobility(EComponentMobility::Movable);
	DoraemonMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -72.0f));
	DoraemonMeshComponent->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	DoraemonMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DoraemonMeshComponent->SetGenerateOverlapEvents(false);
	DoraemonMeshComponent->SetCastShadow(true);
	DoraemonMeshComponent->SetReceivesDecals(false);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> DoraemonMeshAsset(
		TEXT("/Game/Assets/Blender/characters/SM_Doraemon_Showcase.SM_Doraemon_Showcase")
	);
	if (DoraemonMeshAsset.Succeeded())
	{
		DoraemonMeshComponent->SetStaticMesh(DoraemonMeshAsset.Object);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load SM_Doraemon_Showcase for the hero character."));
	}

	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->MaxWalkSpeed = 480.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2048.0f;
}

void AWarriorHeroCharacter::BeginPlay()
{
	Super::BeginPlay();

	// The blueprint still owns a skeletal hero mesh for animation experiments.
	// Hide it in game so only the Doraemon showcase mesh is visible.
	GetMesh()->SetVisibility(false, true);
	GetMesh()->SetHiddenInGame(true);

	if (DoraemonMeshComponent)
	{
		DoraemonMeshComponent->SetVisibility(true, true);
		DoraemonMeshComponent->SetHiddenInGame(false);
	}

	Debug::Log(TEXT("Hero Character Begin Play"));
}
