// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the instance
	// The spring arm component has some collision checking for us built in so that the camera never sticks through the wall.
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	// Make sure that the spring arm is properly attached to the character
	SpringArmComp->SetupAttachment(RootComponent);

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
	// Specify in which direction to move (uses vectors)
	AddMovementInput(GetActorForwardVector(),Value);
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

	// Adds Yaw as input (horizontal rotation)
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);

}

