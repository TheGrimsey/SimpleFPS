// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleFPSPlayerState.h"

#include "SimpleFPSPlayerController.h"

#include "UnrealNetwork.h"

ASimpleFPSPlayerState::ASimpleFPSPlayerState() : APlayerState()
{
	PrimaryActorTick.bCanEverTick = false;

	SetShouldUpdateReplicatedPing(false);
}

void ASimpleFPSPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASimpleFPSPlayerState, Team);
	DOREPLIFETIME(ASimpleFPSPlayerState, NoTeam);

	DOREPLIFETIME(ASimpleFPSPlayerState, Deaths);
	DOREPLIFETIME(ASimpleFPSPlayerState, Kills);
}

void ASimpleFPSPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	if (ASimpleFPSPlayerState * NewSimpleFPSPlayerState = Cast<ASimpleFPSPlayerState>(PlayerState))
	{
		NewSimpleFPSPlayerState->Team = Team;
		NewSimpleFPSPlayerState->NoTeam = NoTeam;

		NewSimpleFPSPlayerState->Kills = Kills;
		NewSimpleFPSPlayerState->Deaths = Deaths;
	}

}

void ASimpleFPSPlayerState::ResetKD()
{
	Kills = 0;
	Deaths = 0;

	ForceNetUpdate();
}

void ASimpleFPSPlayerState::OnGotKill(ASimpleFPSPlayerState* KilledCharacter)
{
	++Kills;

	if (HasAuthority())
	{
		ClientOnGotKill(KilledCharacter);
	}
}

void ASimpleFPSPlayerState::OnDeath(ASimpleFPSPlayerState* Killer)
{
	++Deaths;

	if (HasAuthority())
	{
		ClientOnDeath(Killer);
	}
}

void ASimpleFPSPlayerState::OnRespawn()
{
	if (HasAuthority())
	{
		ClientOnRespawn();
	}
}

void ASimpleFPSPlayerState::ClientOnGotKill_Implementation(ASimpleFPSPlayerState* KilledCharacter)
{	
	OnCharacterKill.Broadcast(this, KilledCharacter);
}

void ASimpleFPSPlayerState::ClientOnDeath_Implementation(ASimpleFPSPlayerState* Killer)
{
	OnCharacterDeath.Broadcast(this, Killer);
}

void ASimpleFPSPlayerState::ClientOnRespawn_Implementation()
{	
	OnCharacterRespawn.Broadcast(this);
}
