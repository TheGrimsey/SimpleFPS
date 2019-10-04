// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleFPSPlayerController.h"

#include "Engine/World.h"
#include "SimpleFPSGameModeBase.h"

void ASimpleFPSPlayerController::OnCharacterDeath()
{
    UnPossess();
    
    ASimpleFPSGameModeBase* FPSGamemode = GetWorld()->GetAuthGameMode<ASimpleFPSGameModeBase>();
    if (FPSGamemode)
    {
        FPSGamemode->OnPlayerDeath(this);
    }

    ++Deaths;
}
