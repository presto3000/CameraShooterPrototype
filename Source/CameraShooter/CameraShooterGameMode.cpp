// Copyright Epic Games, Inc. All Rights Reserved.

#include "CameraShooterGameMode.h"
#include "CameraShooterCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACameraShooterGameMode::ACameraShooterGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
