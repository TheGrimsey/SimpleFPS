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
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void InitGameState() override;

    void OnPlayerDeath(class ASimpleFPSPlayerController* Player, APawn* Pawn);

	virtual void PostLogin(APlayerController* PlayerController) override;

	virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal = TEXT(""));

	UFUNCTION(BlueprintCallable)
	void StartMatch();

	UFUNCTION(BlueprintCallable)
	void ServerTravel(const FString MapName);

	UFUNCTION(BlueprintCallable)
	void KickPlayer(APlayerController* PlayerToKick, const FText& KickReason);

	UFUNCTION()
	void OnTeamGainedKill(int Team, int NewKillCount);

    /*
    *   Variables
    */
public:
    //How long we leave the corpse of a player before deleting it.
    UPROPERTY(EditAnywhere)
    float CorpseLifeTime = 5.f;

    //How long the player stays dead before respawning. TODO
    UPROPERTY(EditAnywhere)
    float RespawnTime = 5.f;

	//Amount of teams in the match.
	UPROPERTY(EditDefaultsOnly)
	int Teams = 4;

	//This is how many kills a team/player must get for the game to end.
	UPROPERTY(EditDefaultsOnly)
	int KillGoal = 10;
};
