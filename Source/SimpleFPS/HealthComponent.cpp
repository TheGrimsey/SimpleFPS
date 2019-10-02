// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "UnrealNetwork.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
    // Disable ticking for this Component.
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
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
