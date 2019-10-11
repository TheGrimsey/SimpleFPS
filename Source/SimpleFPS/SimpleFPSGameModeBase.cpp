// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleFPSGameModeBase.h"

#include "SimpleFPSPlayerController.h"

void ASimpleFPSGameModeBase::OnPlayerDeath(ASimpleFPSPlayerController* Player, APawn* Pawn)
{
    //Set the old pawn (now corpse) to be killed after CorpseLifeTime.
    Pawn->SetLifeSpan(CorpseLifeTime);
}
