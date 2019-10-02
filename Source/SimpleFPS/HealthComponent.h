// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPLEFPS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

    //Handles deciding what properties to replicated to who.
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
    //Sets our Current & Max Health to the input value.
    void InitHealth(float InitalHealthValue);

    /*
    *   Variables
    */
protected:
    //Our current health.
    UPROPERTY(EditAnywhere, Replicated)
    float CurrentHealth;

    //The max health we can have.
    UPROPERTY(EditAnywhere, Replicated)
    float MaxHealth;
};
