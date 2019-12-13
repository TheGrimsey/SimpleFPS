// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleFPSPlayerState.h"

#include "UnrealNetwork.h"

void ASimpleFPSPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASimpleFPSPlayerState, Team);
}
