// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "TimerManager.h"
#include "Engine/World.h"
#include "WorldCollision.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HealthComponent.h"

#include "SimpleFPSPlayerController.h"
#include "SimpleFPSPlayerState.h"
#include "SimpleFPSGameState.h"

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
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    SetRootComponent(SphereCollider);
}

void AProjectile::Init(ASimpleFPSPlayerState* SourcePlayerState, float StartVelocity)
{
	SourceState = SourcePlayerState;
	Team = SourcePlayerState->Team;

	ProjectileMovement->SetVelocityInLocalSpace(FVector::ForwardVector * StartVelocity);
	SetForwardVelocity(StartVelocity);

	OverlapDelegate.BindUObject(this, &AProjectile::HandleAsyncExplosionOverlap);
}

void AProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AProjectile, Team);
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
	static FName FunctionName = FName(TEXT("HandleAsyncExplosionOverlap"));

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);

	GetWorld()->AsyncOverlapByChannel(GetActorLocation(), GetActorRotation().Quaternion(), ECollisionChannel::ECC_GameTraceChannel1, FCollisionShape::MakeSphere(ExplosionRadius), CollisionQueryParams, FCollisionResponseParams(), &OverlapDelegate);
}

void AProjectile::HandleAsyncExplosionOverlap(const FTraceHandle& TraceHandle, FOverlapDatum& OverlapDatum)
{
	//Loop through all objects we hit.
	for (FOverlapResult Hit : OverlapDatum.OutOverlaps)
	{
		if (Hit.Actor == nullptr) continue;

		//Deal damage.
		UHealthComponent* TargetHealthComponent = Cast<UHealthComponent>(Hit.Actor->GetComponentByClass(UHealthComponent::StaticClass()));
		if (TargetHealthComponent)
		{
			APawn* HitPawn = Cast<APawn>(Hit.Actor);
			ASimpleFPSPlayerState* HitPlayerState = nullptr;
			if (HitPawn)
			{
				HitPlayerState = HitPawn->GetPlayerState<ASimpleFPSPlayerState>();
			}

			//Deal damage to target and if they die because of it we notify our sourcecharacter (if it is still valid).
			if (TargetHealthComponent->Damage(ExplosionDamage, SourceState.Get()))
			{
				/*
				*	Check if hit actor is teammate if so let's not grant a kill credit for it.
				*/
				bool bShouldGetKillCredit = HitPlayerState && HitPlayerState->Team != Team;

				if (bShouldGetKillCredit)
				{
					if (SourceState.IsValid())
					{
						SourceState.Get()->OnGotKill(HitPlayerState);
					}

					if (ASimpleFPSGameState* FPSGameState = GetWorld()->GetGameState<ASimpleFPSGameState>())
					{
						FPSGameState->AddKillForTeam(Team);
					}
				}
			}
		}

		//Apply explosion force.
		UCharacterMovementComponent* CharacterMoveComp = Cast<UCharacterMovementComponent>(Hit.Actor->GetComponentByClass(UCharacterMovementComponent::StaticClass()));
		if (CharacterMoveComp)
		{
			CharacterMoveComp->AddRadialImpulse(GetActorLocation(), ExplosionRadius, ExplosionForce, ERadialImpulseFalloff::RIF_Constant, true);
		}
	}

	Destroy();
}

void AProjectile::OnRep_Team()
{
	BP_OnRep_Team();
}

void AProjectile::SetForwardVelocity_Implementation(float Velocity)
{
    ProjectileMovement->SetVelocityInLocalSpace(FVector::ForwardVector * Velocity);
}

