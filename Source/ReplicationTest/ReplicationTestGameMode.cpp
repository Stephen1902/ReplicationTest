// Copyright Epic Games, Inc. All Rights Reserved.

#include "ReplicationTestGameMode.h"
#include "ReplicationTestHUD.h"
#include "ReplicationTestCharacter.h"
#include "UObject/ConstructorHelpers.h"

AReplicationTestGameMode::AReplicationTestGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AReplicationTestHUD::StaticClass();
}
