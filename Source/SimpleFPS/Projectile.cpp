// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "TimerManager.h"
#include "Engine/World.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "HealthComponent.h"


// Sets default values
AProjectile::AProjectile()
{
 	// Disable ticking for this Actor.
	PrimaryActorTick.bCanEverTick = false;

    bReplicates = true;
    bReplicateMovement = true;

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

    SetRootComponent(MeshComponent);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AProjectile::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
    //Set explosion timer.
    GetWorldTimerManager().SetTimer(ExplosionTimer, FTimerDelegate::CreateUObject(this, &AProjectile::Explode), ExploisionDelay, false);
}

void AProjectile::Explode()
{

    TArray<FHitResult> Hits;
    if(GetWorld()->SweepMultiByChannel(Hits, GetActorLocation(), GetActorLocation() + 0.01f, GetActorRotation().Quaternion(), ECollisionChannel::ECC_WorldDynamic, FCollisionShape::MakeSphere(ExplosionRadius)))
    {
        //Loop through all objects we hit.
        for (FHitResult Hit : Hits)
        {
            //TODO: Check so we don't hit something behind a wall.
            UHealthComponent* TargetHealthComponent = Cast<UHealthComponent>(Hit.Actor->GetComponentByClass(UHealthComponent::StaticClass()));
            if (TargetHealthComponent)
            {
                TargetHealthComponent->Damage(Damage);
            }
        }
    }
}

void AProjectile::SetForwardVelocity(float Velocity)
{
    ProjectileMovement->SetVelocityInLocalSpace(FVector::ForwardVector * Velocity);
}

