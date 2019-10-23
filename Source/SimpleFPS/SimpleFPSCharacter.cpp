// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleFPSCharacter.h"

#include "SimpleFPSPlayerController.h"

#include "HealthComponent.h"
#include "GunComponent.h"
#include "WeaponAsset.h"

#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"

#include "UnrealNetwork.h"

// Sets default values
ASimpleFPSCharacter::ASimpleFPSCharacter()
{
 	// We don't want this actor to tick.
    PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bCanEverTick = false;

    // Rotate Yaw (left&right) based on controller rotation.
    bUseControllerRotationYaw = true;

    /*
    *   Initialize Components.
    */

    //Initialize CapsuleCollider size.
    GetCapsuleComponent()->InitCapsuleSize(40.f, 90.f);
    //Make the CapsuleComponent our root object.
    SetRootComponent(GetCapsuleComponent());

    //Initialize CharacterMovement Component
    //Set Jump Velocity.
    GetCharacterMovement()->JumpZVelocity = 600.f;
    //Set amount of movement control in the air.
    GetCharacterMovement()->AirControl = 0.5f;

    HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
    //Init Health to 100.
    HealthComponent->InitHealth(100.f);

    //Bind the OnHealthChanged event to our OnHealthChanged function.
    {
        FScriptDelegate OnHealthChangedDelegate;
        OnHealthChangedDelegate.BindUFunction(this, FName(TEXT("OnHealthChanged")));
        HealthComponent->OnHealthChanged.Add(OnHealthChangedDelegate);
    }

    GunComponent = CreateDefaultSubobject<UGunComponent>(TEXT("Gun Component"));
    //Bind OnWeaponChanged event to our OnWeaponChanged function.
    {
        FScriptDelegate OnWeaponChangedDelegate;
        OnWeaponChangedDelegate.BindUFunction(this, FName(TEXT("OnWeaponChanged")));
        GunComponent->OnWeaponInfoChanged.Add(OnWeaponChangedDelegate);
    }

    /*
    *   Set up GunMesh;
    */
    GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun Mesh"));
    GunMesh->SetupAttachment(GetMesh(), TEXT("RightHand"));

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
    //Attach Camera to capsule component.
    CameraComponent->SetupAttachment(GetCapsuleComponent());
    //Put the Camera 80 units above the middle of the character.
    CameraComponent->RelativeLocation = FVector(0.f, 0.f, 80.f);
    //Rotate Camera pitch based on controller rotation.
    CameraComponent->bUsePawnControlRotation = true;
}

void ASimpleFPSCharacter::BeginPlay()
{
	/*
	*	Make sure we start with our weapon mesh.
	*/
	if (GunComponent->GetEquippedWeapon() && GunComponent->GetEquippedWeapon()->GetMesh())
	{
		GunMesh->SkeletalMesh = GunComponent->GetEquippedWeapon()->GetMesh();
	}

	Super::BeginPlay();
}

void ASimpleFPSCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ASimpleFPSCharacter, HealthComponent);

    DOREPLIFETIME(ASimpleFPSCharacter, GunComponent);
}

// Called to bind functionality to input
void ASimpleFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASimpleFPSCharacter::OnFirePressed);
    PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASimpleFPSCharacter::OnFireReleased);

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

    PlayerInputComponent->BindAxis("MoveForward", this, &ASimpleFPSCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ASimpleFPSCharacter::MoveRight);

    PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

}

void ASimpleFPSCharacter::OnFirePressed()
{
    GunComponent->LocalOnFirePressed();
}

void ASimpleFPSCharacter::OnFireReleased()
{
    GunComponent->LocalOnFireReleased();
}

void ASimpleFPSCharacter::MoveForward(float Value)
{
    //Check so we have a valid player controller and aren't trying to move nowhere.
    if ((Controller != nullptr) && (Value != 0.0f))
    {
        // Find out which way is forward
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // Get forward vector
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

        // Add movement in that direction.
        AddMovementInput(Direction, Value);
    }
}

void ASimpleFPSCharacter::MoveRight(float Value)
{
    //Check so we have a valid player controller and aren't trying to move nowhere.
    if ((Controller != nullptr) && (Value != 0.0f))
    {
        // Find out which way is right
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // Get right vector 
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        // Add movement in that direction
        AddMovementInput(Direction, Value);

    }
}

FTransform ASimpleFPSCharacter::GetFireTransform()
{
    //Rotation. Just our controller rotation.
    FRotator Rotation = GetControlRotation();

    //Location. We add our capsule radius onto it so it doesn't get stuck in us.
    FVector Location = CameraComponent->GetComponentLocation() + GetControlRotation().RotateVector(FVector(GetCapsuleComponent()->GetScaledCapsuleRadius(), 0.f, 0.f));

    return FTransform(Rotation, Location);
}

void ASimpleFPSCharacter::OnHealthChanged(float NewHealth, float OldHealth)
{
    if (NewHealth <= 0.f)
    {
        OnDeath();
    }
}

void ASimpleFPSCharacter::OnDeath()
{
    //Notify controller of our death.
    ASimpleFPSPlayerController* FPSController = Cast<ASimpleFPSPlayerController>(GetController());
    if (FPSController)
    {
        FPSController->OnPawnDeath();
    }

    //Clear all the listeners from our health.
    HealthComponent->OnHealthChanged.Clear();

	BP_OnDeath();
}

void ASimpleFPSCharacter::OnWeaponChanged(const UWeaponAsset* NewWeapon)
{
    GunMesh->SkeletalMesh = NewWeapon->GetMesh();
}

