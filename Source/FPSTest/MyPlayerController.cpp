// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSTestCharacter.h"
#include "MyPlayerController.h"
#include "GameFramework/GameMode.h"

void AMyPlayerController::OnKilled()
{
	UnPossess();
	GetWorldTimerManager().SetTimer(TimerHandle_Respawn, this, &AMyPlayerController::Respawn, 5.f);
}

void AMyPlayerController::Respawn()
{
	AGameModeBase * GameMode = GetWorld()->GetAuthGameMode();
		
	if (GameMode)
	{
		APawn* NewPawn = GameMode->SpawnDefaultPawnFor(this, GameMode->ChoosePlayerStart(this));
		Possess(NewPawn);
	}
}
