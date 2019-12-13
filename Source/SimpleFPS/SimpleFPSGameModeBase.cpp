// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleFPSGameModeBase.h"
#include "SimpleFPSPlayerState.h"

#include "GameFramework/GameState.h"
#include "Kismet/GameplayStatics.h" 
#include "UnrealNetwork.h"

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

	TeamKills.Init(0, Teams);
	TeamDeaths.Init(0, Teams);
}

void ASimpleFPSGameModeBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ASimpleFPSGameModeBase, Teams, COND_InitialOnly);

	DOREPLIFETIME(ASimpleFPSGameModeBase, TeamKills);
	DOREPLIFETIME(ASimpleFPSGameModeBase, TeamDeaths);
}

void ASimpleFPSGameModeBase::OnPlayerDeath(ASimpleFPSPlayerController* Player, APawn* Pawn)
{
    //Set the old pawn (now corpse) to be killed after CorpseLifeTime.
    Pawn->SetLifeSpan(CorpseLifeTime);
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

void ASimpleFPSGameModeBase::OnPlayerDeath(ASimpleFPSPlayerState* KilledPlayer, ASimpleFPSPlayerState* Killer)
{
	AddKillForTeam(Killer->Team);
}
