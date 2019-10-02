// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleFPSCharacter.h"

#include "HealthComponent.h"
#include "GunComponent.h"

#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"

#include "UnrealNetwork.h"

// Sets default values
ASimpleFPSCharacter::ASimpleFPSCharacter()
{
 	// We don't want this actor to tick.
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

    GunComponent = CreateDefaultSubobject<UGunComponent>(TEXT("Gun Component"));

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
    //Attach Camera to capsule component.
    CameraComponent->SetupAttachment(GetCapsuleComponent());
    //Put the Camera 80 units above the middle of the character.
    CameraComponent->RelativeLocation = FVector(0.f, 0.f, 80.f);
    //Rotate Camera pitch based on controller rotation.
    CameraComponent->bUsePawnControlRotation = true;
}

// Called when the game starts or when spawned
void ASimpleFPSCharacter::BeginPlay()
{
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

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

    PlayerInputComponent->BindAxis("MoveForward", this, &ASimpleFPSCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ASimpleFPSCharacter::MoveRight);

    PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

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

