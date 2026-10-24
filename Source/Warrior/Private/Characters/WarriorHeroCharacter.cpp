// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/WarriorHeroCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "WarriorDebugHelper.h"

DEFINE_LOG_CATEGORY_STATIC(LogWarriorMovement, Log, All);

AWarriorHeroCharacter::AWarriorHeroCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a spring arm component
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 300.0f; // The camera follows at this distance behind the character
	SpringArmComponent->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	SpringArmComponent->SocketOffset = FVector(0.0f, 50.0f, 50.0f); // Offset the camera above the character
	SpringArmComponent->bDoCollisionTest = true;
	// Create a camera component
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);

	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2048.0f;
}

void AWarriorHeroCharacter::BeginPlay()
{
	Super::BeginPlay();
	ApplyCameraCollisionSettings();
	Debug::Log(TEXT("Hero Character Begin Play"));
}

void AWarriorHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UE_LOG(LogWarriorMovement, Log, TEXT("Hero movement input component ready."));

	PlayerInputComponent->BindAxis(
		TEXT("Move Forward / Backward"),
		this,
		&ThisClass::MoveForward);
	PlayerInputComponent->BindAxis(
		TEXT("Move Right / Left"),
		this,
		&ThisClass::MoveRight);
	PlayerInputComponent->BindAxis(
		TEXT("Turn Right / Left Mouse"),
		this,
		&ThisClass::Turn);
	PlayerInputComponent->BindAxis(
		TEXT("Turn Right / Left Gamepad"),
		this,
		&ThisClass::Turn);
	PlayerInputComponent->BindAxis(
		TEXT("Look Up / Down Mouse"),
		this,
		&ThisClass::LookUp);
	PlayerInputComponent->BindAxis(
		TEXT("Look Up / Down Gamepad"),
		this,
		&ThisClass::LookUp);
	PlayerInputComponent->BindAction(
		TEXT("Jump"),
		IE_Pressed,
		this,
		&ThisClass::StartJump);
	PlayerInputComponent->BindAction(
		TEXT("Jump"),
		IE_Released,
		this,
		&ThisClass::StopJump);
}

void AWarriorHeroCharacter::MoveForward(float Value)
{
	if (!Controller || FMath::IsNearlyZero(Value))
	{
		return;
	}
	if (!bMovementInputObserved)
	{
		bMovementInputObserved = true;
		UE_LOG(LogWarriorMovement, Log, TEXT("Movement input accepted: %.3f"), Value);
	}

	const FRotator YawRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
	AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X), Value);
}

void AWarriorHeroCharacter::MoveRight(float Value)
{
	if (!Controller || FMath::IsNearlyZero(Value))
	{
		return;
	}
	if (!bMovementInputObserved)
	{
		bMovementInputObserved = true;
		UE_LOG(LogWarriorMovement, Log, TEXT("Movement input accepted: %.3f"), Value);
	}

	const FRotator YawRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
	AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y), Value);
}

void AWarriorHeroCharacter::Turn(float Value)
{
	if (!FMath::IsNearlyZero(Value))
	{
		AddControllerYawInput(Value);
	}
}

void AWarriorHeroCharacter::LookUp(float Value)
{
	if (!FMath::IsNearlyZero(Value))
	{
		AddControllerPitchInput(Value);
	}
}

void AWarriorHeroCharacter::StartJump()
{
	Jump();
}

void AWarriorHeroCharacter::StopJump()
{
	StopJumping();
}

void AWarriorHeroCharacter::ApplyCameraCollisionSettings()
{
	if (SpringArmComponent)
	{
		SpringArmComponent->ProbeSize = CameraCollisionProbeSize;
	}
}
