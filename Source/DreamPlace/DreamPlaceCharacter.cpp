// Copyright Epic Games, Inc. All Rights Reserved.

#include "DreamPlaceCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Bullet.h"

//////////////////////////////////////////////////////////////////////////
// ADreamPlaceCharacter

ADreamPlaceCharacter::ADreamPlaceCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ADreamPlaceCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ADreamPlaceCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ADreamPlaceCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ADreamPlaceCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ADreamPlaceCharacter::LookUpAtRate);

	// Shoot
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &ADreamPlaceCharacter::Shoot);
	PlayerInputComponent->BindAction("Shoot", IE_Released, this, &ADreamPlaceCharacter::StopShooting);

	// Fire
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ADreamPlaceCharacter::Fire);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ADreamPlaceCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ADreamPlaceCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ADreamPlaceCharacter::OnResetVR);
}


void ADreamPlaceCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ADreamPlaceCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ADreamPlaceCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ADreamPlaceCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ADreamPlaceCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ADreamPlaceCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ADreamPlaceCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ADreamPlaceCharacter::Shoot()
{
	IsInShooting = true;
}

void ADreamPlaceCharacter::StopShooting()
{
	IsInShooting = false;
}

void ADreamPlaceCharacter::Fire()
{
	if (ProjectileClass)
	{
		// Get camera position
		FVector CharacterLocation;
		FRotator CharacterRotation;
		
		//USkeletalMeshComponent *GunMeshComponent = GetMesh();
		//GunMeshComponent->GetSocketWorldLocationAndRotation(TEXT("gun_Socket"), CharacterLocation, CharacterRotation);
		
		CharacterLocation = GetActorLocation();
		// Adjuct to the gun's location
		CharacterLocation += FVector(0.0f, 0.0f, 65.0f);
		CharacterRotation = GetActorRotation();
		//GetActorEyesViewPoint(CameraLocation, CameraRotation);

		// Change muzzle offset from camera space to world space
		FVector MuzzleLocation = CharacterLocation + FTransform(CharacterRotation).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = CharacterRotation;

		MuzzleRotation.Pitch += 0.0f;
		UWorld* World = GetWorld();

		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator(); // Instigater is private
			// Generate the bullet
			ABullet* Bullet = World->SpawnActor<ABullet>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Bullet)
			{
				// Set bullet's initial direction
				FVector LaunchDirection = MuzzleRotation.Vector();
				Bullet->FireInDirection(LaunchDirection);
			}
		}
	}
}

