// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleFPSPlayerController.h"

#include "Engine/World.h"
#include "SimpleFPSGameModeBase.h"
#include "SimpleFPSCharacter.h"

void ASimpleFPSPlayerController::OnPawnDeath()
{
    //Leave body for 30 seconds before removing it.
    GetPawn()->SetLifeSpan(30.f);

    UnPossess();
    
    ASimpleFPSGameModeBase* FPSGamemode = GetWorld()->GetAuthGameMode<ASimpleFPSGameModeBase>();
    if (FPSGamemode)
    {
        FPSGamemode->OnPlayerDeath(this);
    }

    ++Deaths;

    //Schedule respawn.
}

void ASimpleFPSPlayerController::OnPossess(APawn* pawn)
{
    //Register OnDeathdelegate.
    if (ASimpleFPSCharacter* SimpleFPSChar = Cast<ASimpleFPSCharacter>(pawn))
    {

    }
}
