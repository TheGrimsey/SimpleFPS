// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DELEGATE_OneParam(FOnHealthChangedDelegate, float)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPLEFPS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
    //Handles deciding what properties to replicated to who.
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
    //Sets our Current & Max Health to the input value.
    void InitHealth(float InitalHealthValue);

    //Apply Damage removing health.
    void Damage(float Damage);

    //Apply healing adding health.
    void Heal(float Healing);

    /*
    *   Variables
    */
public:
    //UPROPERTY(BlueprintAssignable)
    //FOnHealthChangedDelegate OnHealthChanged;

protected:
    //Our current health.
    UPROPERTY(EditAnywhere, Replicated, BlueprintReadOnly)
    float CurrentHealth;

    //The max health we can have.
    UPROPERTY(EditAnywhere, Replicated, BlueprintReadOnly)
    float MaxHealth;
};
