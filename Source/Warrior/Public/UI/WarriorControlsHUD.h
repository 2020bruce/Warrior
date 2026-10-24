#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "WarriorControlsHUD.generated.h"

UCLASS()
class WARRIOR_API AWarriorControlsHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

private:
	void DrawLockOnReticle(float LayoutScale);

	UPROPERTY(EditDefaultsOnly, Category = "Controls", meta = (ClampMin = "1.0"))
	float ControlsDisplaySeconds = 30.0f;
};
