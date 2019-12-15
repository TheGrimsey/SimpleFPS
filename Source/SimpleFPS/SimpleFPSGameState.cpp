// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleFPSGameState.h"

#include "UnrealNetwork.h"

void ASimpleFPSGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ASimpleFPSGameState, Teams, COND_InitialOnly);

	DOREPLIFETIME(ASimpleFPSGameState, TeamKills);
	DOREPLIFETIME(ASimpleFPSGameState, TeamDeaths);
	DOREPLIFETIME(ASimpleFPSGameState, WinningTeam);
}

void ASimpleFPSGameState::AddPlayerState(APlayerState* PlayerState)
{
	OnPlayerStateAdded.Broadcast(PlayerState);

	Super::AddPlayerState(PlayerState);
}

void ASimpleFPSGameState::RemovePlayerState(APlayerState* PlayerState)
{
	OnPlayerStateRemoved.Broadcast(PlayerState);

	Super::RemovePlayerState(PlayerState);
}
