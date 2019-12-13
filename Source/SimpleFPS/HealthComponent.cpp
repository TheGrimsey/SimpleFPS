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

bool UHealthComponent::Damage(float Damage, ASimpleFPSPlayerState* Attacker)
{
	if (IsAlive())
	{
		//Remove the absolute damage
		ModHealth(-FMath::Abs(Damage), Attacker);

		return !IsAlive();
	}

	//We were already dead so return false.
	return false;
}

void UHealthComponent::Heal(float Healing, ASimpleFPSPlayerState* Healer)
{
	//Only accept healing if we are still alive.
	if (IsAlive())
	{
		//Add the absolute healing value.
		ModHealth(FMath::Abs(Healing), Healer);
	}
}
