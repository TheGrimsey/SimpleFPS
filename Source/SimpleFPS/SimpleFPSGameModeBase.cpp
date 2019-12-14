// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleFPSGameModeBase.h"
#include "SimpleFPSPlayerState.h"

#include "SimpleFPSGameState.h"
#include "Kismet/GameplayStatics.h" 
#include "UnrealNetwork.h"
#include "GameFramework/GameSession.h"
#include "Engine/World.h"

#include "SimpleFPSPlayerController.h"


void ASimpleFPSGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	//Parse teams.
	FString TeamCount = UGameplayStatics::ParseOption(Options, TEXT("TeamCount"));
	if (!TeamCount.IsEmpty() && FCString::IsNumeric(*TeamCount))
	{
		Teams = FCString::Atoi(*TeamCount);
	}
}

void ASimpleFPSGameModeBase::InitGameState()
{
	Super::InitGameState();

	if (ASimpleFPSGameState * FPSGameState = GetGameState<ASimpleFPSGameState>())
	{
		FPSGameState->Teams = Teams;
		FPSGameState->TeamKills.Init(0, Teams);
		FPSGameState->TeamDeaths.Init(0, Teams);
	}
}

void ASimpleFPSGameModeBase::OnPlayerDeath(ASimpleFPSPlayerController* Player, APawn* Pawn)
{
    //Set the old pawn (now corpse) to be killed after CorpseLifeTime.
	if (Pawn)
	{
		Pawn->SetLifeSpan(CorpseLifeTime);
	}
}

void ASimpleFPSGameModeBase::PostLogin(APlayerController* PlayerController)
{
	Super::PostLogin(PlayerController);

	if (ASimpleFPSPlayerState* NewPlayerState = PlayerController->GetPlayerState<ASimpleFPSPlayerState>())
	{
		/*
		*	Register death callbacks.
		*/
		FScriptDelegate DeathDelegate;
		DeathDelegate.BindUFunction(this, TEXT("OnPlayerDeath"));

		NewPlayerState->OnCharacterDeath.Add(DeathDelegate);

		/*
		*	SET PLAYER TEAM!
		*/

		//Free for all. Just have team at 0.
		if (Teams == 0)
		{
			NewPlayerState->Team = 0;
			NewPlayerState->NoTeam = true;

			return;
		}

		//Determine team with least players
		TArray<int> MembersInEachTeam = TArray<int>();
		MembersInEachTeam.Init(0, Teams);

		//Count all team members
		if (GetGameState<AGameStateBase>())
		{
			for (APlayerState* PlayerState : GetGameState<AGameStateBase>()->PlayerArray)
			{
				if (NewPlayerState == PlayerState) continue;

				if (ASimpleFPSPlayerState* FPSPlayerState = Cast<ASimpleFPSPlayerState>(PlayerState))
				{
					MembersInEachTeam[FPSPlayerState->Team]++;
				}
			}
		}

		//Find smallest team.
		int SmallestTeam = 0;

		for (int i = 0; i < MembersInEachTeam.Num(); ++i)
		{
			if (MembersInEachTeam[i] < MembersInEachTeam[SmallestTeam])
			{
				SmallestTeam = i;
			}
		}

		//Assign player to smallest team;
		NewPlayerState->Team = SmallestTeam;
	}
}

FString ASimpleFPSGameModeBase::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal)
{
	FString returnString = Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);

	FString PlayerName = UGameplayStatics::ParseOption(Options, TEXT("PlayerName"));
	if (!PlayerName.IsEmpty())
	{
		NewPlayerController->PlayerState->SetPlayerName(PlayerName);
	}

	return returnString;
}

void ASimpleFPSGameModeBase::ServerTravel(const FString MapName)
{
	GetWorld()->ServerTravel(MapName);
}

void ASimpleFPSGameModeBase::KickPlayer(APlayerController* PlayerToKick, const FText& KickReason)
{
	GameSession->KickPlayer(PlayerToKick, KickReason);
}

void ASimpleFPSGameModeBase::OnPlayerDeath(ASimpleFPSPlayerState* KilledPlayer, ASimpleFPSPlayerState* Killer)
{

}
