// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponAsset.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class SIMPLEFPS_API UWeaponAsset : public UDataAsset
{
	GENERATED_BODY()
	
	/*
	*	Variables	
	*/
	UPROPERTY(EditAnywhere)
	class USkeletalMesh* Mesh = nullptr;

	UPROPERTY(EditAnywhere)
	int32 MaxAmmunition = 0;

	UPROPERTY(EditAnywhere)
	bool bAutomaticFire = false;

	UPROPERTY(EditAnywhere)
	float TimeBetweenShots = 0.f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile> Projectile = nullptr;

	UPROPERTY(EditAnywhere)
	float ProjectileSpeed = 1000.f;

	/*
	*	Methods
	*/
public:
	UFUNCTION(BlueprintPure)
	class USkeletalMesh* GetMesh() const
	{
		return Mesh;
	}

	UFUNCTION(BlueprintPure)
	int32 GetMaxAmmunition() const
	{
		return MaxAmmunition;
	}

	UFUNCTION(BlueprintPure)
	bool IsAutomaticFire() const
	{
		return bAutomaticFire;
	}

	UFUNCTION(BlueprintPure)
	float GetTimeBetweenShots() const
	{
		return TimeBetweenShots;
	}

	UFUNCTION(BlueprintPure)
	TSubclassOf<class AProjectile> GetProjectileActor() const
	{
		return Projectile;
	}

	UFUNCTION(BlueprintPure)
	float GetProjectileSpeed() const
	{
		return ProjectileSpeed;
	}
};
