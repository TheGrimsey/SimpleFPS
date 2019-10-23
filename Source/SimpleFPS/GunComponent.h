// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GunComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponInfoChanged, const class UWeaponAsset*, NewWeapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponFired, const class UWeaponAsset*, WeaponUsed);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPLEFPS_API UGunComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGunComponent();

protected:
    //Handles deciding what properties to replicated to who.
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UFUNCTION(BlueprintCallable)
	void AddAmmo(int32 Amount);

    //Returns true if we pass all requirements to fire.
    bool CanFire();

    //Called when we press the fire button locally.
    void LocalOnFirePressed();

    //Called when we release the fire button locally.
    void LocalOnFireReleased();

    //Called on the server when we press the fire button.
    UFUNCTION(Server, Reliable)
    void ServerOnFirePressed();

    //Called on the server when we release the fire button.
    UFUNCTION(Server, Reliable)
    void ServerOnFireReleased();

    //Handles actually firing, creating projectile, etc.
    void Fire();

    //Tells clients that someone has fired.
    UFUNCTION(NetMulticast, Reliable)
    void MulticastOnFire();

    //Handles the visuals.
    void OnFire();

    UFUNCTION(BlueprintPure)
    int32 GetCurrentAmmunition()
    {
        return CurrentAmmunition;
    }

    UFUNCTION(BlueprintPure)
    class UWeaponAsset* GetEquippedWeapon()
    {
        return EquippedWeapon;
    }

    UFUNCTION()
    void OnRep_EquippedWeapon();

    /*
    *   Variables
    */
public:
	UPROPERTY(BlueprintAssignable)
    FOnWeaponInfoChanged OnWeaponInfoChanged;

	UPROPERTY(BlueprintAssignable)
	FOnWeaponFired OnWeaponFired;

protected:
    UPROPERTY(EditAnywhere, Replicated, ReplicatedUsing=OnRep_EquippedWeapon)
    class UWeaponAsset* EquippedWeapon;

    UPROPERTY(EditAnywhere, Replicated)
    int32 CurrentAmmunition = 0;

    UPROPERTY(VisibleAnywhere)
    float TimeOfLastShot = 0.f;

    UPROPERTY(VisibleAnywhere)
    FTimerHandle AutomaticFireTimer;
};
