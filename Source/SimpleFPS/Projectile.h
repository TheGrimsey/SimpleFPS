// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class SIMPLEFPS_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	void Init(class ASimpleFPSPlayerState* SourcePlayerState, float StartVelocity);

protected:
    //Handles deciding what properties to replicated to who.
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // Called when the projectile collides with something.
    UFUNCTION()
    void OnHit(const FHitResult& ImpactResult, const FVector& ImpactVelocity);

    // Called to blow up the projectile.
    void Explode();

	UFUNCTION()
	void OnRep_Team();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnRep_Team();

    UFUNCTION(NetMulticast, Reliable)
    void SetForwardVelocity(float Velocity);

    /*
    *   Variables
    */
public:
    UPROPERTY(EditAnywhere)
    class UProjectileMovementComponent* ProjectileMovement;

    UPROPERTY(EditAnywhere)
    class USphereComponent* SphereCollider;

    UPROPERTY(EditAnywhere)
    class UStaticMeshComponent* MeshComponent;

protected:
    //Amount of damage the explosion from this projectile does.
    UPROPERTY(EditDefaultsOnly)
    float ExplosionDamage;

    //The radius of this projectile's explosion.
    UPROPERTY(EditDefaultsOnly)
    float ExplosionRadius;

    //The force applied to actors when the projectile explodes.
    UPROPERTY(EditDefaultsOnly)
    float ExplosionForce;

    //Delay in seconds from that the projectile hits something that it explodes.
    UPROPERTY(EditDefaultsOnly)
    float ExploisionDelay;

	/*
	*	INSTANCED
	*/
    //The character that fired this projectile.
    UPROPERTY(VisibleInstanceOnly)
    TWeakObjectPtr<class ASimpleFPSPlayerState> SourceState;

	UPROPERTY(VisibleInstanceOnly, ReplicatedUsing=OnRep_Team)
	int Team;

protected:
    //Timer for the explosion delay.
    UPROPERTY(VisibleInstanceOnly)
    FTimerHandle ExplosionTimer;

    //True if we have hit something and are ramping up to explode.
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
    bool bIsFuseLit;
};
