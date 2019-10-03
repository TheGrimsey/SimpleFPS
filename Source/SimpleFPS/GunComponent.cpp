// Fill out your copyright notice in the Description page of Project Settings.


#include "GunComponent.h"

#include "UnrealNetwork.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Components/SphereComponent.h"

#include "Projectile.h"
#include "SimpleFPSCharacter.h"

// Sets default values for this component's properties
UGunComponent::UGunComponent()
{
    // Disable ticking for this Component.
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void UGunComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UGunComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UGunComponent, CurrentGunInformation);

    DOREPLIFETIME(UGunComponent, CurrentAmmunition);
    
    DOREPLIFETIME(UGunComponent, TimeOfLastShot);
}

bool UGunComponent::CanFire()
{
    //Check so we have ammunition and the shot 'cooldown' has passed.
    return CurrentAmmunition > 0 && (GetWorld()->GetTimeSeconds() - CurrentGunInformation.TimeBetweenShots) > TimeOfLastShot;
}

void UGunComponent::LocalOnFirePressed()
{
    if (CanFire())
    {
        ServerOnFirePressed();
    }
}

void UGunComponent::LocalOnFireReleased()
{
    ServerOnFireReleased();
}

void UGunComponent::ServerOnFirePressed_Implementation()
{
    Fire();

    if (CurrentGunInformation.bAutomaticFire)
    {
        GetWorld()->GetTimerManager().SetTimer(AutomaticFireTimer, FTimerDelegate::CreateUObject(this, &UGunComponent::Fire), CurrentGunInformation.TimeBetweenShots, false);
    }

}

void UGunComponent::ServerOnFireReleased_Implementation()
{
    GetWorld()->GetTimerManager().ClearTimer(AutomaticFireTimer);
}

void UGunComponent::Fire()
{
    if (CanFire())
    {
        ASimpleFPSCharacter* Owner = Cast<ASimpleFPSCharacter>(GetOwner());

        FTransform ProjectileTransform = Owner->GetFireTransform();

        FVector ProjectileCollisionOffset = ProjectileTransform.GetRotation().RotateVector(FVector(CurrentGunInformation.Projectile.GetDefaultObject()->SphereCollider->GetScaledSphereRadius(), 0.f, 0.f));
        ProjectileTransform.SetLocation(ProjectileTransform.GetLocation() + ProjectileCollisionOffset);

        AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(CurrentGunInformation.Projectile, ProjectileTransform);
        if (Projectile)
        {
            Projectile->SetForwardVelocity(CurrentGunInformation.ProjectileSpeed);

            CurrentAmmunition--;
        }
    }

    MulticastOnFire();
}

void UGunComponent::MulticastOnFire_Implementation()
{
    OnFire();
}

void UGunComponent::OnFire()
{
    UE_LOG(LogTemp, Log, TEXT("Fired!"))
}
