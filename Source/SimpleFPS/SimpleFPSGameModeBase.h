// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SimpleFPSGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEFPS_API ASimpleFPSGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    void OnPlayerDeath(class ASimpleFPSPlayerController* Player);
};
