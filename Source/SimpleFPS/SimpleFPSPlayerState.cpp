// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleFPSPlayerState.h"

#include "SimpleFPSPlayerController.h"

#include "UnrealNetwork.h"

void ASimpleFPSPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASimpleFPSPlayerState, Team);

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

void ASimpleFPSPlayerState::Reset()
{
	Kills = 0;
	Deaths = 0;

	Super::Reset();
}

void ASimpleFPSPlayerState::OnGotKill(ASimpleFPSPlayerState* KilledCharacter)
{
	++Kills;

	if (HasAuthority())
	{
		OnCharacterKill.Broadcast(this, KilledCharacter);
		ClientOnGotKill(KilledCharacter);
	}
}

void ASimpleFPSPlayerState::OnDeath(ASimpleFPSPlayerState* Killer)
{
	++Deaths;
	if (HasAuthority())
	{
		OnCharacterDeath.Broadcast(this, Killer);

		ClientOnDeath(Killer);
	}
}

void ASimpleFPSPlayerState::OnRespawn()
{
	if (HasAuthority())
	{
		OnCharacterRespawn.Broadcast(this);
	}
}

void ASimpleFPSPlayerState::ClientOnGotKill_Implementation(ASimpleFPSPlayerState* KilledCharacter)
{
	if (HasAuthority()) return;

	OnCharacterKill.Broadcast(this, KilledCharacter);
}

void ASimpleFPSPlayerState::ClientOnDeath_Implementation(ASimpleFPSPlayerState* Killer)
{
	if (HasAuthority()) return;

	OnCharacterDeath.Broadcast(this, Killer);
}

void ASimpleFPSPlayerState::ClientOnRespawn_Implementation()
{
	if (HasAuthority()) return;

	OnCharacterRespawn.Broadcast(this);
}
