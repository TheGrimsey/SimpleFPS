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
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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

    /*
    *   Variables
    */
protected:
    UPROPERTY(EditAnywhere, Replicated)
    class UHealthComponent* HealthComponent;

    UPROPERTY(EditAnywhere, Replicated)
    class UGunComponent* GunComponent;

    UPROPERTY(EditAnywhere)
    class UCameraComponent* CameraComponent;

};
