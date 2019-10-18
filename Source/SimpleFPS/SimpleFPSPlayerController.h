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

    /*
    *   Methods
    */
public:
	//Handles deciding what properties to replicated to who.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    //Called when our character has died.
    UFUNCTION()
    void OnPawnDeath();

    //Called when our character has respawned.
    UFUNCTION()
    void OnPawnRespawn();

    //RPC to call OnPlayerRespawn on client.
    UFUNCTION(Client, Reliable)
    void ClientOnPawnRespawn();

	UFUNCTION()
	void OnPawnGotKill();

protected:
    UFUNCTION()
    void Respawn();

    /*
    *   Variables
    */
public:
    UPROPERTY(BlueprintAssignable)
    FOnCharacterDeath OnCharacterDeath;

    UPROPERTY(BlueprintAssignable)
    FOnCharacterRespawn OnCharacterRespawn;

protected:
    UPROPERTY()
    struct FTimerHandle RespawnTimer;

    //Amount of times we have died.
    UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadOnly)
    int32 Deaths = 0;

    //Amount of kills we have. Not specifically enemies.
    UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadOnly)
    int32 Kills = 0;
};
