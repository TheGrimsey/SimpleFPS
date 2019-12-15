// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SimpleFPSGameInstance.generated.h"

UENUM()
enum class EGameState : uint8
{
	None,
	GoingToMatch,
	ReturningLobby
};

/**
 * 
 */
UCLASS()
class SIMPLEFPS_API USimpleFPSGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	/*
	*	Variables
	*/
public:
	UPROPERTY()
	int LastWinner = -1;

	UPROPERTY()
	TArray<int> LastTeamKills;

	UPROPERTY()
	TArray<int> LastTeamDeaths;

	UPROPERTY(VisibleAnywhere)
	EGameState State;

};
