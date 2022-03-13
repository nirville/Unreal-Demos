 // Fill out your copyright notice in the Description page of Project Settings.

#include "FPSTest.h"
#include "FPSTestGameMode.h"
#include "MyPlayerState.h"
#include "MyPlayerController.h"
#include "Net/UnrealNetwork.h"


void AMyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyPlayerState, bTeamB);
}