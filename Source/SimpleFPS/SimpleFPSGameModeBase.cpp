// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleFPSGameModeBase.h"

#include "SimpleFPSPlayerController.h"

#include "Engine/World.h"

void ASimpleFPSGameModeBase::OnPlayerDeath(ASimpleFPSPlayerController* Player)
{
    AActor* RespawnLocation = ChoosePlayerStart(Player);

    APawn* NewPlayerCharacter = GetWorld()->SpawnActor<APawn>(DefaultPawnClass, RespawnLocation->GetTransform());

    Player->Possess(NewPlayerCharacter);
    
    /*
    *   In the future we may want to make a note that the player has died.
    */
}
