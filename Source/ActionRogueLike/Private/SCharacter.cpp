// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the instance
	// The spring arm component has some collision checking for us built in so that the camera never sticks through the wall.
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	// Allows the user to move the camera without moving the pawn
	// Boleans start with b
	SpringArmComp->bUsePawnControlRotation = true;
	// Make sure that the spring arm is properly attached to the character
	SpringArmComp->SetupAttachment(RootComponent);

	//  Rotate our character to whatever we're currently moving towards (Gets rids of the sideway movement.)
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// Yaw is the horizontal rotation
	bUseControllerRotationYaw = false;

	// Create the instance
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	// Attach the camera to the spring (hence 3rd person)
	CameraComp->SetupAttachment(SpringArmComp);


}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Moves the character forward/backwards. Values from -1 to 0 and from 0 to 1 (Image a joystick)
// Move Forward 1, Move Backwards -1
// Bind key in: Edit/Project Settings/Engine/Inputs/Axis Bindings/"MoveForward"(the name from Setup)
void ASCharacter::MoveForward(float Value)
{
	// Move into the the direction of the camera instead of the direction of the pawn. E.g If the camera looked left then
	// we want to move in that direction and just rotate the character in that direction.
	FRotator ControlRot = GetControlRotation();

	// Make sure that we don't accidently mess with our input where we try to move into the floor or into the sky.
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	// Specify in which direction to move (uses vectors, not rotators)
	AddMovementInput(ControlRot.Vector(),Value);
}

void ASCharacter::MoveRight(float Value)
{
	//Moves right regardless of the rotation of our pawn but instead based on the rotation of our camera
	// You get the Y (right vector) of the camera and use that for the A and D.

	// Vectors direction
	// X = Forward (Red)
	// Y = Right (Green)
	// Z = Up (Blue)

	FRotator ControlRot = GetControlRotation();
	// Make sure that we don't accidently mess with our input where we try to move into the floor or into the sky.
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, Value);
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	// Adds Yaw as input (horizontal rotation)
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

}



