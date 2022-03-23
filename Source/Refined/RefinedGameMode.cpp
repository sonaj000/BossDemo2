// Copyright Epic Games, Inc. All Rights Reserved.

#include "RefinedGameMode.h"
#include "RefinedCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARefinedGameMode::ARefinedGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
