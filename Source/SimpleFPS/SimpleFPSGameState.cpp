// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleFPSGameState.h"

#include "UnrealNetwork.h"

void ASimpleFPSGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ASimpleFPSGameState, KillGoal, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(ASimpleFPSGameState, Teams, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(ASimpleFPSGameState, WinningTeam, COND_InitialOnly);

	DOREPLIFETIME(ASimpleFPSGameState, TeamKills);
	DOREPLIFETIME(ASimpleFPSGameState, TeamDeaths);
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

void ASimpleFPSGameState::MulticastSendServerMessage_Implementation(const FText& Message)
{
	OnServerMessage.Broadcast(Message);
}
