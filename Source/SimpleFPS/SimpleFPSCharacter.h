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

	virtual void BeginPlay() override;

	virtual void OnRep_PlayerState();

    //Handles deciding what properties to replicated to who.
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//Returns the transform from which our bullets should come from.
	FTransform GetFireTransform();

protected:
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

    UFUNCTION()
    void OnHealthChanged(float NewHealth, float OldHealth, class ASimpleFPSPlayerState* Changer);

    void OnDeath(class ASimpleFPSPlayerState* Killer);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnDeath(class ASimpleFPSPlayerState* Killer);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnRepPlayerState();

    UFUNCTION()
    void OnWeaponChanged(const class UWeaponAsset* NewWeapon);

	UFUNCTION()
	void RegenerateAmmo();
    /*
    *   Variables
    */
public:
    UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Replicated)
    class UHealthComponent* HealthComponent;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;

    UPROPERTY(Category = Weapons, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Replicated)
    class UGunComponent* GunComponent;

    UPROPERTY(Category = Weapons, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class USkeletalMeshComponent* GunMesh;
    
	UPROPERTY(Category = Weapons, EditDefaultsOnly)
	float TimeBetweenAmmoRegen = 3;

	UPROPERTY(Category = Weapons, EditDefaultsOnly)
	int MinAmmoRegenAmount = 2;

	UPROPERTY(Category = Weapons, EditDefaultsOnly)
	int MaxAmmoRegenAmount = 4;
};
