#include "UI/WarriorControlsHUD.h"

#include "Controllers/WarriorHeroController.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "Engine/Font.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"

void AWarriorControlsHUD::DrawHUD()
{
	Super::DrawHUD();

	if (!Canvas || !GetWorld())
	{
		return;
	}

	const float LayoutScale = FMath::Clamp(Canvas->ClipX / 1600.0f, 0.75f, 1.35f);
	DrawLockOnReticle(LayoutScale);

	if (GetWorld()->GetTimeSeconds() > ControlsDisplaySeconds)
	{
		return;
	}

	const float PanelX = 24.0f * LayoutScale;
	const float PanelY = 72.0f * LayoutScale;
	const float PanelWidth = 560.0f * LayoutScale;
	const float PanelHeight = 210.0f * LayoutScale;
	const float LineHeight = 27.0f * LayoutScale;
	const float TextX = PanelX + 18.0f * LayoutScale;
	float TextY = PanelY + 16.0f * LayoutScale;
	UFont* Font = GEngine ? GEngine->GetSmallFont() : nullptr;

	DrawRect(
		FLinearColor(0.025f, 0.03f, 0.025f, 0.86f),
		PanelX,
		PanelY,
		PanelWidth,
		PanelHeight);
	DrawText(
		TEXT("WARRIOR CONTROLS"),
		FLinearColor(0.92f, 0.68f, 0.36f, 1.0f),
		TextX,
		TextY,
		Font,
		1.2f * LayoutScale);
	TextY += LineHeight * 1.45f;

	const TCHAR* Lines[] = {
		TEXT("Move: WASD / Left Stick"),
		TEXT("Camera: Mouse / Right Stick"),
		TEXT("Jump: Space / Gamepad A"),
		TEXT("Lock On: Middle Mouse / R3"),
		TEXT("Switch Target: Wheel, Q, E / D-Pad"),
	};
	for (const TCHAR* Line : Lines)
	{
		DrawText(
			Line,
			FLinearColor(0.9f, 0.9f, 0.86f, 1.0f),
			TextX,
			TextY,
			Font,
			LayoutScale);
		TextY += LineHeight;
	}
}

void AWarriorControlsHUD::DrawLockOnReticle(float LayoutScale)
{
	AWarriorHeroController* WarriorController = Cast<AWarriorHeroController>(PlayerOwner);
	AActor* Target = WarriorController ? WarriorController->GetLockOnTarget() : nullptr;
	if (!IsValid(Target))
	{
		return;
	}

	FVector BoundsOrigin;
	FVector BoundsExtent;
	Target->GetActorBounds(false, BoundsOrigin, BoundsExtent);
	const FVector FocusLocation = BoundsOrigin + FVector(0.0f, 0.0f, BoundsExtent.Z * 0.35f);

	FVector2D ScreenLocation;
	if (!WarriorController->ProjectWorldLocationToScreen(FocusLocation, ScreenLocation, true))
	{
		return;
	}

	const float MarkerSize = 18.0f * LayoutScale;
	const float MarkerGap = 7.0f * LayoutScale;
	const float MarkerThickness = FMath::Max(1.0f, 2.0f * LayoutScale);
	const FLinearColor MarkerColor(1.0f, 0.62f, 0.18f, 1.0f);

	DrawLine(
		ScreenLocation.X - MarkerGap - MarkerSize,
		ScreenLocation.Y,
		ScreenLocation.X - MarkerGap,
		ScreenLocation.Y,
		MarkerColor,
		MarkerThickness);
	DrawLine(
		ScreenLocation.X + MarkerGap,
		ScreenLocation.Y,
		ScreenLocation.X + MarkerGap + MarkerSize,
		ScreenLocation.Y,
		MarkerColor,
		MarkerThickness);
	DrawLine(
		ScreenLocation.X,
		ScreenLocation.Y - MarkerGap - MarkerSize,
		ScreenLocation.X,
		ScreenLocation.Y - MarkerGap,
		MarkerColor,
		MarkerThickness);
	DrawLine(
		ScreenLocation.X,
		ScreenLocation.Y + MarkerGap,
		ScreenLocation.X,
		ScreenLocation.Y + MarkerGap + MarkerSize,
		MarkerColor,
		MarkerThickness);
	DrawRect(MarkerColor, ScreenLocation.X - 2.0f, ScreenLocation.Y - 2.0f, 4.0f, 4.0f);
}
