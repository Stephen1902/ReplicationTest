// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ReplicationTestHUD.generated.h"

UCLASS()
class AReplicationTestHUD : public AHUD
{
	GENERATED_BODY()

public:
	AReplicationTestHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

