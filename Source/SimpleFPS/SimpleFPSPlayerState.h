// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SimpleFPSPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEFPS_API ASimpleFPSPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	int Team = 0;

	//Returns true if the other player is friendly to us.
	UFUNCTION(BlueprintCallable)
	bool IsFriendly(ASimpleFPSPlayerState* Player)
	{
		return Player->Team == Team;
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
