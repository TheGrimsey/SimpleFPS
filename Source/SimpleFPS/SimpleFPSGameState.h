// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SimpleFPSGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateAddedRemoved, class APlayerState*, PlayerState);

/**
 * 
 */
UCLASS()
class SIMPLEFPS_API ASimpleFPSGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

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

	UFUNCTION(BlueprintPure)
	FORCEINLINE int GetTeamKills(int Team)
	{
		return TeamKills[Team];
	}

	UFUNCTION(BlueprintPure)
	FORCEINLINE int GetTeamDeaths(int Team)
	{
		return TeamDeaths[Team];
	}

	/** Add PlayerState to the PlayerArray */
	virtual void AddPlayerState(APlayerState* PlayerState);

	/** Remove PlayerState from the PlayerArray. */
	virtual void RemovePlayerState(APlayerState* PlayerState);

public:
	UPROPERTY(BlueprintAssignable)
	FOnPlayerStateAddedRemoved OnPlayerStateAdded;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStateAddedRemoved OnPlayerStateRemoved;

	UPROPERTY(EditDefaultsOnly, Replicated)
	int Teams = 4;

	UPROPERTY(VisibleInstanceOnly, Replicated)
	TArray<int> TeamKills;

	UPROPERTY(VisibleInstanceOnly, Replicated)
	TArray<int> TeamDeaths;
};
