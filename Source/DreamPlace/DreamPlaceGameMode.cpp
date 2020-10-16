// Copyright Epic Games, Inc. All Rights Reserved.

#include "DreamPlaceGameMode.h"
#include "DreamPlaceCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADreamPlaceGameMode::ADreamPlaceGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
