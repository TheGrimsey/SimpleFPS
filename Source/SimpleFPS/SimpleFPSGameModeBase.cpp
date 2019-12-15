// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleFPSGameModeBase.h"
#include "SimpleFPSPlayerState.h"

#include "SimpleFPSGameState.h"
#include "SimpleFPSGameInstance.h"
#include "Kismet/GameplayStatics.h" 
#include "UnrealNetwork.h"
#include "GameFramework/GameSession.h"
#include "Engine/World.h"

#include "SimpleFPSPlayerController.h"

ASimpleFPSGameModeBase::ASimpleFPSGameModeBase()
{
	GameStateClass = ASimpleFPSGameState::StaticClass();
	PlayerStateClass = ASimpleFPSPlayerState::StaticClass();

	bUseSeamlessTravel = true;
}

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

	UE_LOG(LogTemp, Log, TEXT("INIT GAMESTATE"))

	if (ASimpleFPSGameState * FPSGameState = GetGameState<ASimpleFPSGameState>())
	{
		FPSGameState->Teams = Teams;
		FPSGameState->TeamKills.Init(0, Teams);
		FPSGameState->TeamDeaths.Init(0, Teams);

		{        
			FScriptDelegate OnTeamKillGainedDelegate;
			OnTeamKillGainedDelegate.BindUFunction(this, FName(TEXT("OnTeamGainedKill")));
			FPSGameState->OnTeamGainedKill.Add(OnTeamKillGainedDelegate);
		}

		if (USimpleFPSGameInstance * GameInstance = GetGameInstance<USimpleFPSGameInstance>())
		{
			if (GameInstance->State == EGameState::ReturningLobby)
			{
				FPSGameState->TeamKills = GameInstance->LastTeamKills;
				FPSGameState->TeamDeaths = GameInstance->LastTeamDeaths;
				FPSGameState->WinningTeam = GameInstance->LastWinner;
			}
		}
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

void ASimpleFPSGameModeBase::StartMatch()
{
	/*
	*	Reset player kills & deaths.
	*/
	for (APlayerState* PlayerState : GetGameState<AGameStateBase>()->PlayerArray)
	{
		if (ASimpleFPSPlayerState * FPSPlayerState = Cast<ASimpleFPSPlayerState>(PlayerState))
		{
			FPSPlayerState->ResetKD();
		}
	}
	if (USimpleFPSGameInstance * GameInstance = GetGameInstance<USimpleFPSGameInstance>())
	{
		GameInstance->State = EGameState::GoingToMatch;
	}

	//Go to game map.
	ServerTravel(TEXT("TestMap"));
}

void ASimpleFPSGameModeBase::ServerTravel(const FString MapName)
{

	GetWorld()->ServerTravel(MapName, false);
}

void ASimpleFPSGameModeBase::KickPlayer(APlayerController* PlayerToKick, const FText& KickReason)
{
	GameSession->KickPlayer(PlayerToKick, KickReason);
}

void ASimpleFPSGameModeBase::OnTeamGainedKill(int Team, int NewKillCount)
{
	if (NewKillCount >= KillGoal)
	{
		EndMatch(Team);
	}
}

void ASimpleFPSGameModeBase::EndMatch(int WinningTeam)
{
	if (USimpleFPSGameInstance * GameInstance = GetGameInstance<USimpleFPSGameInstance>())
	{
		//Save teamkills and deaths.
		if (ASimpleFPSGameState * FPSGameState = GetGameState<ASimpleFPSGameState>())
		{
			GameInstance->LastTeamKills = FPSGameState->TeamKills;
			GameInstance->LastTeamDeaths = FPSGameState->TeamDeaths;
		}

		//Save victor.
		GameInstance->LastWinner = WinningTeam;

		UE_LOG(LogTemp, Log, TEXT("Setting state to returning to lobby."))
		GameInstance->State = EGameState::ReturningLobby;
	}

	//Return to lobby.
	ServerTravel(TEXT("Lobby"));
}
