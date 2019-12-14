// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SimpleFPSPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRepPlayerState);

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

	virtual void ClientSetHUD_Implementation(TSubclassOf<class AHUD> NewHUDClass) override;

	virtual void OnRep_PlayerState();

	virtual void GetSeamlessTravelActorList(bool bToEntry, TArray<class AActor*>& ActorList);

protected:
    UFUNCTION()
    void Respawn();

    /*
    *   Variables
    */
public:
	UPROPERTY(BlueprintAssignable)
	FOnRepPlayerState OnRepPlayerState;

protected:
	UPROPERTY()
	TSubclassOf<class AHUD> HudClass;

    UPROPERTY()
    struct FTimerHandle RespawnTimer;
};
