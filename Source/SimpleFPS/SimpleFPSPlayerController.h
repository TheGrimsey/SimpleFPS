// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SimpleFPSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEFPS_API ASimpleFPSPlayerController : public APlayerController
{
    GENERATED_BODY()

    void OnCharacterDeath();

    /*
    *   Variables
    */
protected:
    //Amount of times we have died.
    uint32 Deaths;

    //Amount of kills we have. Not specifically enemies.
    uint32 Kills;
};
