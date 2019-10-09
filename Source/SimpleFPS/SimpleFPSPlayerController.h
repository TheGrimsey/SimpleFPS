// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SimpleFPSPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterRespawn);

/**
 * 
 */
UCLASS()
class SIMPLEFPS_API ASimpleFPSPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    void OnPawnDeath();

protected:
    virtual void OnPossess(APawn* pawn) override;

    /*
    *   Variables
    */
public:
    UPROPERTY(BlueprintAssignable)
    FOnCharacterDeath OnCharacterDeath;

    UPROPERTY(BlueprintAssignable)
    FOnCharacterRespawn OnCharacterRespawn;

protected:
    //Amount of times we have died.
    UPROPERTY(VisibleAnywhere)
    uint32 Deaths = 0;

    //Amount of kills we have. Not specifically enemies.
    UPROPERTY(VisibleAnywhere)
    uint32 Kills = 0;
};
