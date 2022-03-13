// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPSTestGameMode.h"
#include "FPSTestHUD.h"
#include "FPSTestCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "MyPlayerController.h"
#include "MyPlayerState.h"
#include "MyPlayerStart.h"

AFPSTestGameMode::AFPSTestGameMode(const FObjectInitializer & ObjectInitializer)
	: Super(ObjectInitializer)
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSTestHUD::StaticClass();

	PlayerControllerClass = AMyPlayerController::StaticClass();
	PlayerStateClass = AMyPlayerState::StaticClass();
}

/*
void AFPSTestGameMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (NewPlayer)
	{
		AMyPlayerState * PS = Cast<AMyPlayerState>(NewPlayer->PlayerState);

		if (PS && GameState)
		{
			uint8 NumTeamA = 0;
			uint8 NumTeamB = 0;

			for (AMyPlayerState * It : GameState->PlayerArray)
			{
				AMyPlayerState * OtherPS = Cast<AMyPlayerState>(It);
				if (OtherPS)
				{
					if (OtherPS->bTeamB)
					{
						NumTeamB++;
					}
					else
					{
						NumTeamA++;
					}
				}
			}

			if (NumTeamA > NumTeamB)
			{
				PS->bTeamB = true;
			}
		}
	}
}
*/

/*
AActor * AFPSTestGameMode::ChoosePlayerStart(AController * Player)
{
	if (Player)
	{
		AMyPlayerState* PS = Cast<AMyPlayerState>(Player->PlayerState);

		if (PS)
		{
			TArray<AMyPlayerStart *> Starts;

			for (TActorIterator<AMyPlayerStart> StartItr(GetWorld()); StartItr; ++StartItr)
			{
				if (StartItr->bTeamB == PS->bTeamB)
				{
					Starts.Add(*StartItr);
				}
			}

			return Starts[FMath::RandRange(0, Starts.Num() - 1)];
		}
	}

	return NULL;
}
*/
