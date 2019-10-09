// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "TimerManager.h"
#include "Engine/World.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HealthComponent.h"

#include "UnrealNetwork.h"

// Sets default values
AProjectile::AProjectile()
{
    // Disable ticking for this Actor.
    PrimaryActorTick.bCanEverTick = false;

    bReplicates = true;
    bReplicateMovement = true;

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));

    FScriptDelegate OnHitDelegate;
    OnHitDelegate.BindUFunction(this, FName(TEXT("OnHit")));

    ProjectileMovement->OnProjectileBounce.Add(OnHitDelegate);

    SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));
    SphereCollider->SetCollisionProfileName(FName(TEXT("BlockAllDynamic")));
    SphereCollider->SetSphereRadius(25.f);

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    MeshComponent->SetupAttachment(SphereCollider);

    SetRootComponent(SphereCollider);
}

void AProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AProjectile, ProjectileMovement);
}

void AProjectile::OnHit(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
    //We only want to do things if we have authority.
    if (!HasAuthority()) return;

    //We only want to warm up for an explosion if we aren't already doing that.
    if (!bIsFuseLit)
    {
        //Set explosion timer if we have a delay else just explode directly.
        if (ExploisionDelay > 0.f)
        {
            GetWorldTimerManager().SetTimer(ExplosionTimer, FTimerDelegate::CreateUObject(this, &AProjectile::Explode), ExploisionDelay, false);
        }
        else
        {
            Explode();
        }

        //We are lit!
        bIsFuseLit = true;
    }
}

void AProjectile::Explode()
{
    TArray<FHitResult> Hits;
    if (GetWorld()->SweepMultiByChannel(Hits, GetActorLocation(), GetActorLocation() + 0.01f, GetActorRotation().Quaternion(), ECollisionChannel::ECC_WorldDynamic, FCollisionShape::MakeSphere(ExplosionRadius)))
    {
        //List all actors we have already hit so we don't damage them multiple times.
        TArray<AActor*> ActorsHit;

        //Loop through all objects we hit.
        for (FHitResult Hit : Hits)
        {
            //Check so we haven't hit this actor already.
            if (ActorsHit.Contains(Hit.Actor)) continue;

            //Deal damage.
            UHealthComponent* TargetHealthComponent = Cast<UHealthComponent>(Hit.Actor->GetComponentByClass(UHealthComponent::StaticClass()));
            if (TargetHealthComponent)
            {
                TargetHealthComponent->Damage(Damage);
            }

            //Apply explosion force.
            UCharacterMovementComponent* CharacterMoveComp = Cast<UCharacterMovementComponent>(Hit.Actor->GetComponentByClass(UCharacterMovementComponent::StaticClass()));
            if (CharacterMoveComp)
            {
                CharacterMoveComp->AddRadialImpulse(GetActorLocation(), ExplosionRadius, ExplosionForce, ERadialImpulseFalloff::RIF_Linear, true);
            }
            ActorsHit.Add(Hit.Actor.Get());
        }
    }

    Destroy();
}

void AProjectile::SetForwardVelocity_Implementation(float Velocity)
{
    ProjectileMovement->SetVelocityInLocalSpace(FVector::ForwardVector * Velocity);
}

