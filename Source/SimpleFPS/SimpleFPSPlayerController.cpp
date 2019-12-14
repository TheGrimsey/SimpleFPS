// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleFPSPlayerController.h"
#include "Engine/World.h"

#include "SimpleFPSGameModeBase.h"
#include "SimpleFPSPlayerState.h"

#include "GameFramework/HUD.h"

#include "TimerManager.h"
#include "UnrealNetwork.h"

void ASimpleFPSPlayerController::BeginPlay()
{
	if (ASimpleFPSPlayerState* FPSPlayerState = GetPlayerState<ASimpleFPSPlayerState>())
	{
		{
			FScriptDelegate DeathDelegate;
			DeathDelegate.BindUFunction(this, TEXT("OnPawnDeath"));

			FPSPlayerState->OnCharacterDeath.Add(DeathDelegate);
		}
	} 
}

void ASimpleFPSPlayerController::OnPawnDeath()
{
    if (!HasAuthority()) return;

    //Save our dead pawn.
    APawn* LastPawn = GetPawn();

    UnPossess();

    ASimpleFPSGameModeBase* FPSGamemode = GetWorld()->GetAuthGameMode<ASimpleFPSGameModeBase>();
    if (FPSGamemode)
    {
        //Notify the gamemode that our character died.
        FPSGamemode->OnPlayerDeath(this, LastPawn);

        if (FPSGamemode->RespawnTime > 0.f)
        {
            GetWorldTimerManager().SetTimer(RespawnTimer, FTimerDelegate::CreateUObject(this, &ASimpleFPSPlayerController::Respawn), FPSGamemode->RespawnTime, false);
        }
        else
        {
            Respawn();
        }
    }
}

void ASimpleFPSPlayerController::OnPawnRespawn()
{
	if (ASimpleFPSPlayerState* SimpleFPSState = GetPlayerState<ASimpleFPSPlayerState>())
	{
		SimpleFPSState->OnRespawn();
	}
}

void ASimpleFPSPlayerController::ClientOnPawnRespawn_Implementation()
{
    OnPawnRespawn();
}

void ASimpleFPSPlayerController::ClientSetHUD_Implementation(TSubclassOf<AHUD> NewHUDClass)
{
	if (MyHUD)
	{
		MyHUD->Destroy();
	}

	if (PlayerState)
	{
		Super::ClientSetHUD_Implementation(NewHUDClass);
	}
	else
	{
		HudClass = NewHUDClass;
	}
}

void ASimpleFPSPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	OnRepPlayerState.Broadcast();

	if (HudClass)
	{
		ClientSetHUD_Implementation(HudClass);
	}
}

void ASimpleFPSPlayerController::GetSeamlessTravelActorList(bool bToEntry, TArray<class AActor*>& ActorList)
{
	Super::GetSeamlessTravelActorList(bToEntry, ActorList);

	//Stop HUD from persisting.
	if (MyHUD)
	{
		ActorList.Remove(MyHUD);
	}
}

void ASimpleFPSPlayerController::Respawn()
{
    GetWorld()->GetAuthGameMode<AGameModeBase>()->RestartPlayer(this);

    //Make sure we actually respawned by checking if we controll a pawn.
    if (GetPawn())
    {
        OnPawnRespawn();

        //RPC to tell client we respawned.
        ClientOnPawnRespawn();
    }
}
