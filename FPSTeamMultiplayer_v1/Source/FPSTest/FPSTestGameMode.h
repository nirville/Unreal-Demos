// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSTestGameMode.generated.h"

UCLASS(minimalapi)
class AFPSTestGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFPSTestGameMode(const FObjectInitializer& ObjectInitializer);

	//void PostLogin(APlayerController* NewPlayer) override;

	//AActor * ChoosePlayerStart(AController * Player);

	bool ShouldSpawnAtStartSpot(AController * Player) override { return false; };
};



