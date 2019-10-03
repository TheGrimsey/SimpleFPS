// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GunComponent.generated.h"

USTRUCT()
struct FGunInformation
{
    GENERATED_BODY()

    FGunInformation() {

    }

    UPROPERTY(EditAnywhere)
    class USkeletalMesh* Model;

    UPROPERTY(EditAnywhere)
    uint32 MaxAmmunition;

    UPROPERTY(EditAnywhere)
    bool bAutomaticFire;

    UPROPERTY(EditAnywhere)
    float TimeBetweenShots;

    UPROPERTY(EditAnywhere)
    TSubclassOf<class AProjectile> Projectile;

    UPROPERTY(EditAnywhere)
    float ProjectileSpeed;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPLEFPS_API UGunComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGunComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

    //Handles deciding what properties to replicated to who.
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
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

    /*
    *   Variables
    */
protected:
    UPROPERTY(EditAnywhere, Replicated)
    FGunInformation CurrentGunInformation;

    UPROPERTY(EditAnywhere, Replicated)
    uint32 CurrentAmmunition;

    UPROPERTY(VisibleAnywhere, Replicated)
    float TimeOfLastShot;

    UPROPERTY(VisibleAnywhere)
    FTimerHandle AutomaticFireTimer;
};
