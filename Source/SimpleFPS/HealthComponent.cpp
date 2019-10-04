// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "UnrealNetwork.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
    // Disable ticking for this Component.
	PrimaryComponentTick.bCanEverTick = false;
    bReplicates = true;
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    //Replicated CurrentHealth to everyone.
    DOREPLIFETIME(UHealthComponent, CurrentHealth);

    //Replicate MaxHealth to everyone.
    DOREPLIFETIME(UHealthComponent, MaxHealth);
}

void UHealthComponent::InitHealth(float InitalHealthValue)
{
    CurrentHealth = InitalHealthValue;
    MaxHealth = InitalHealthValue;
}

void UHealthComponent::Damage(float Damage)
{
    //Remove the absolute damage value and also clamp our CurrentHealth between 0 and max. (We don't want to deal negative damage)
    CurrentHealth = FMath::Clamp(CurrentHealth - FMath::Abs(Damage), 0.f, MaxHealth);
}

void UHealthComponent::Heal(float Healing)
{
    //Add the absolute healing value and also clamp our CurrentHealth between 0 and max. (We don't want to do negative healing)
    CurrentHealth = FMath::Clamp(CurrentHealth + FMath::Abs(Healing), 0.f, MaxHealth);
}
