// Copyright Epic Games, Inc. All Rights Reserved.

#include "PoroBotGameMode.h"
#include "PoroBotPawn.h"
//#include "MyHUD.h"

APoroBotGameMode::APoroBotGameMode()
{
	// set default pawn class to our character class
	//DefaultPawnClass = APoroBotPawn::StaticClass();
	DefaultPawnClass = nullptr;
}

