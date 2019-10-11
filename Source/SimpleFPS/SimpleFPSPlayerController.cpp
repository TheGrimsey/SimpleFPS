// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleFPSPlayerController.h"

#include "Engine/World.h"
#include "SimpleFPSGameModeBase.h"
#include "TimerManager.h"

void ASimpleFPSPlayerController::OnPawnDeath()
{
    //Increase our death counter.
    ++Deaths;

    //Broadcast that our character has died.
    OnCharacterDeath.Broadcast();

    if (!HasAuthority()) return;

    //Save our dead pawn.
    APawn* OldPawn = GetPawn();

    UnPossess();

    ASimpleFPSGameModeBase* FPSGamemode = GetWorld()->GetAuthGameMode<ASimpleFPSGameModeBase>();
    if (FPSGamemode)
    {
        //Notify the gamemode that our character died.
        FPSGamemode->OnPlayerDeath(this, OldPawn);

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
    OnCharacterRespawn.Broadcast();
}

void ASimpleFPSPlayerController::ClientOnPawnRespawn_Implementation()
{
    OnPawnRespawn();
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
