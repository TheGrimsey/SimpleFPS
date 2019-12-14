// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SimpleFPSPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterKill, ASimpleFPSPlayerState*, Killer, ASimpleFPSPlayerState*, KilledPlayer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterDeath, ASimpleFPSPlayerState*, KilledPlayer, ASimpleFPSPlayerState*, Killer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterRespawn, ASimpleFPSPlayerState*, RespawnedPlayer);

/**
 * 
 */
UCLASS()
class SIMPLEFPS_API ASimpleFPSPlayerState : public APlayerState
{
	GENERATED_BODY()

	/*
	*	Variables
	*/
public:
	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadOnly)
	bool NoTeam = false;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadOnly)
	int32 Team = 0;

	//Amount of times we have died.
	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadOnly)
	int32 Deaths = 0;

	//Amount of kills we have. Not specifically enemies.
	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadOnly)
	int32 Kills = 0;

	UPROPERTY(BlueprintAssignable)
	FOnCharacterKill OnCharacterKill;

	UPROPERTY(BlueprintAssignable)
	FOnCharacterDeath OnCharacterDeath;

	UPROPERTY(BlueprintAssignable)
	FOnCharacterRespawn OnCharacterRespawn;
	
	/*
	*	Methods
	*/
public:
	//Returns true if the other player is friendly to us.
	UFUNCTION(BlueprintCallable)
	bool IsFriendly(ASimpleFPSPlayerState* Player)
	{
		return !NoTeam || Player->Team == Team;
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void CopyProperties(APlayerState* PlayerState) override;

	virtual void Reset() override;

	void OnGotKill(ASimpleFPSPlayerState* KilledCharacter);

	void OnDeath(ASimpleFPSPlayerState* Killer);

	void OnRespawn();

	UFUNCTION(Client, Reliable)
	void ClientOnGotKill(ASimpleFPSPlayerState* KilledCharacter);

	UFUNCTION(Client, Reliable)
	void ClientOnDeath(ASimpleFPSPlayerState* Killer);

	UFUNCTION(Client, Reliable)
	void ClientOnRespawn();
};
