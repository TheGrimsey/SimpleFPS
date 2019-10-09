// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SimpleFPSCharacter.generated.h"

UCLASS()
class SIMPLEFPS_API ASimpleFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASimpleFPSCharacter();

protected:

    //Handles deciding what properties to replicated to who.
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    //Called when the Fire button is pressed.
    void OnFirePressed();

    //Called when the Fire button is released.
    void OnFireReleased();

    //Handles Forward movement input.
    void MoveForward(float Value);

    //Handles Right movement input.
    void MoveRight(float Value);

    //Returns the transform from which our bullets should come from.
    FTransform GetFireTransform();

    UFUNCTION()
    void OnHealthChanged(float NewHealth, float OldHealth);

    void OnDeath();
    /*
    *   Variables
    */
public:
    UPROPERTY(EditAnywhere, Replicated, BlueprintReadOnly)
    class UHealthComponent* HealthComponent;

    UPROPERTY(EditAnywhere, Replicated, BlueprintReadOnly)
    class UGunComponent* GunComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    class UCameraComponent* CameraComponent;

};
