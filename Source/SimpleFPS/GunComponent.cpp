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
    bReplicates = true;
}

void UGunComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UGunComponent, CurrentGunInformation);

    DOREPLIFETIME_CONDITION(UGunComponent, CurrentAmmunition, COND_OwnerOnly);
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
        OnFire();
    }
}

void UGunComponent::LocalOnFireReleased()
{
    ServerOnFireReleased();
}

void UGunComponent::ServerOnFirePressed_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("Recieved FirePressed from %s."), *GetOwner()->GetName());
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
	UE_LOG(LogTemp, Log, TEXT("%s has %d ammo"), *GetOwner()->GetName(), CurrentAmmunition);
    //Check so we fullfill the requirements to fire.
    if (CanFire())
    {
		UE_LOG(LogTemp, Log, TEXT("%s passed fire requirements."), *GetOwner()->GetName());

        ASimpleFPSCharacter* Owner = Cast<ASimpleFPSCharacter>(GetOwner());

        FTransform ProjectileTransform = Owner->GetFireTransform();

        FVector ProjectileCollisionOffset = ProjectileTransform.GetRotation().RotateVector(FVector(CurrentGunInformation.Projectile.GetDefaultObject()->SphereCollider->GetScaledSphereRadius(), 0.f, 0.f));
        ProjectileTransform.SetLocation(ProjectileTransform.GetLocation() + ProjectileCollisionOffset);

        //Spawn projectile.
        AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(CurrentGunInformation.Projectile, ProjectileTransform);
        if (Projectile)
        {
            //Initialize values in projectile.
            Projectile->SetForwardVelocity(CurrentGunInformation.ProjectileSpeed);
            Projectile->SourceCharacter = Owner;

            //Subtract one piece of ammo from us.
            CurrentAmmunition--;
            //Set the time we last fired a shot to current time.
            TimeOfLastShot = GetWorld()->GetTimeSeconds();

            //Notify everyone that we have fired.
            MulticastOnFire();
        }
    }

}

void UGunComponent::MulticastOnFire_Implementation()
{
    /*
    *   If we are the owning client then we don't want to execute onfire since we have already done it when we sent the fire command.
    */
    if (GetOwnerRole() != ENetRole::ROLE_AutonomousProxy && GetOwner()->GetRemoteRole() != ENetRole::ROLE_SimulatedProxy)
    {
        OnFire();
    }
}

void UGunComponent::OnFire()
{
	FString NameOfOwner = GetOwner()->GetName();
	FString NetworkRole = GetOwnerRole() >= ENetRole::ROLE_AutonomousProxy ? TEXT("locally") : TEXT("Remotely");
	UE_LOG(LogTemp, Log, TEXT("%s fired a shot %s"), *NameOfOwner, *NetworkRole);

	OnWeaponFired.Broadcast(CurrentGunInformation);
}

void UGunComponent::OnRep_CurrentGunInformation()
{
    OnWeaponInfoChanged.Broadcast(CurrentGunInformation);
}
