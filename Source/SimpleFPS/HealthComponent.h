// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHealthChanged, float, NewHealth, float, OldHealth, class ASimpleFPSPlayerState*, Attacker);

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

    //Apply Damage removing health. Returns true if the character dies because of the damage.
    bool Damage(float Damage, class ASimpleFPSPlayerState* Attacker);

    //Apply healing adding health.
    void Heal(float Healing, class ASimpleFPSPlayerState* Healer);

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsAlive()
	{
		return CurrentHealth > 0.f;
	}

private:
    FORCEINLINE void ModHealth(float Modifier, class ASimpleFPSPlayerState* Modder)
    {
        float OldHealth = CurrentHealth;
        CurrentHealth = FMath::Clamp(CurrentHealth + Modifier, 0.f, MaxHealth);

        OnHealthChanged.Broadcast(CurrentHealth, OldHealth, Modder);
    }
    /*
    *   Variables
    */
public:
    UPROPERTY(BlueprintAssignable)
    FOnHealthChanged OnHealthChanged;

protected:
    //Our current health.
    UPROPERTY(EditAnywhere, Replicated, BlueprintReadOnly)
    float CurrentHealth;

    //The max health we can have.
    UPROPERTY(EditAnywhere, Replicated, BlueprintReadOnly)
    float MaxHealth;
};
