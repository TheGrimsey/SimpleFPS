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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    // Called when the projectile collides with something.
    virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit);

    // Called to blow up the projectile.
    void Explode();

public:
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
    float Damage;

    //The radius of this projectile's explosion.
    UPROPERTY(EditDefaultsOnly)
    float ExplosionRadius;

    //Delay in seconds from that the projectile hits something that it explodes.
    UPROPERTY(EditDefaultsOnly)
    float ExploisionDelay;

    //Timer for the explosion delay.
    UPROPERTY(VisibleInstanceOnly)
    FTimerHandle ExplosionTimer;
};
