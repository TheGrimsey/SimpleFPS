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
    void OnPlayerDeath(class ASimpleFPSPlayerController* Player, APawn* Pawn);

	virtual void PostLogin(APlayerController* PlayerController) override;

    /*
    *   Variables
    */
public:
    //How long we leave the corpse of a player before deleting it.
    UPROPERTY(EditAnywhere)
    float CorpseLifeTime = 10.f;

    //How long the player stays dead before respawning. TODO
    UPROPERTY(EditAnywhere)
    float RespawnTime = 5.f;

	UPROPERTY(EditAnywhere)
	int Teams = 4;
};
