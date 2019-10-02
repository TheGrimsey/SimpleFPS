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

    /*
    *   Variables
    */
protected:
    //Amount of damage the explosion from this projectile does.
    UPROPERTY(EditAnywhere)
    float Damage;

    //The radius of this projectile's explosion.
    UPROPERTY(EditAnywhere)
    float ExplosionRadius;

    //Delay in seconds from that the projectile hits something that it explodes.
    UPROPERTY(EditAnywhere)
    float ExploisionDelay;

};
