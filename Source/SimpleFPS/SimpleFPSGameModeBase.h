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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    void OnPlayerDeath(class ASimpleFPSPlayerController* Player, APawn* Pawn);

	virtual void PostLogin(APlayerController* PlayerController) override;

	FORCEINLINE bool IsFFA()
	{
		return Teams == 0;
	}

	FORCEINLINE void AddKillForTeam(int Team)
	{
		TeamKills[Team]++;
	}

	FORCEINLINE void AddDeathForTeam(int Team)
	{
		TeamDeaths[Team]++;
	}

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

	UPROPERTY(EditDefaultsOnly, Replicated)
	int Teams = 4;

	UPROPERTY(VisibleInstanceOnly, Replicated)
	TArray<int> TeamKills;

	UPROPERTY(VisibleInstanceOnly, Replicated)
	TArray<int> TeamDeaths;
};
