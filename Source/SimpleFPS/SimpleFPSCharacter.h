// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "GunComponent.h"

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

protected:
    UFUNCTION()
    void OnHealthChanged(float NewHealth, float OldHealth);

    void OnDeath();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnDeath();

    UFUNCTION()
    void OnWeaponChanged(const struct FGunInformation& NewWeapon);

    /*
    *   Variables
    */
public:
    UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Replicated)
    class UHealthComponent* HealthComponent;

    UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Replicated)
    class UGunComponent* GunComponent;

    UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class USkeletalMeshComponent* GunMesh;
    
    UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* CameraComponent;
};
