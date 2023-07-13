// Copyright Epic Games, Inc. All Rights Reserved.

#include "ExampleGameMode.h"
#include "ExampleHUD.h"
#include "ExampleCharacter.h"
#include "UObject/ConstructorHelpers.h"

AExampleGameMode::AExampleGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AExampleHUD::StaticClass();
}
