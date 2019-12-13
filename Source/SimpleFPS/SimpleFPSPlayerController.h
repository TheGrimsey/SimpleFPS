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

    /*
    *   Methods
    */
public:
	virtual void BeginPlay() override;

    //Called when our character has died.
    UFUNCTION()
    void OnPawnDeath();

    //Called when our character has respawned.
    UFUNCTION()
    void OnPawnRespawn();

    //RPC to call OnPlayerRespawn on client.
    UFUNCTION(Client, Reliable)
    void ClientOnPawnRespawn();

protected:
    UFUNCTION()
    void Respawn();

    /*
    *   Variables
    */
public:


protected:
    UPROPERTY()
    struct FTimerHandle RespawnTimer;
};
