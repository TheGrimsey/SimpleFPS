// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleFPSPlayerState.h"

#include "UnrealNetwork.h"

void ASimpleFPSPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASimpleFPSPlayerState, Team);

	DOREPLIFETIME(ASimpleFPSPlayerState, Deaths);
	DOREPLIFETIME(ASimpleFPSPlayerState, Kills);
}

void ASimpleFPSPlayerState::OnGotKill(ASimpleFPSPlayerState* KilledCharacter)
{
	++Kills;

	OnCharacterKill.Broadcast(this, KilledCharacter);
}

void ASimpleFPSPlayerState::OnDeath(ASimpleFPSPlayerState* Killer)
{
	++Deaths;

	OnCharacterDeath.Broadcast(this, Killer);
}

void ASimpleFPSPlayerState::OnRespawn()
{
	OnCharacterRespawn.Broadcast(this);
}
