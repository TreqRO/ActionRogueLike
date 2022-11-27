// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	// Allows us to assign a class (it could be an actor, pawn, character or in this case our Magic Projectile because Magic Projectile
	// derives from actor.
	// Make sure to add an UPROPERTY to make sure that this is visible from the editor and that we can actually assign a variable.
	UPROPERTY(EditAnywhere)
	// MAKE SURE TO ASSIGN THIS IN BLUEPRINT (Click on the PlayerCharacter BluePrint and search for "ProjectileClass" on the write side and assign the MagicProjectileBP (blue print))
	TSubclassOf<AActor> ProjectileClass;

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:

	//Allows you the see all it's properties in the Engine.
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp; // 3rd person character "Spring" component

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Moves the character forward/backwards. Values from -1 to 0 and from 0 to 1 (Image a joystick)
	void MoveForward(float Value);

	// Move the character left/right
	void MoveRight(float Value);

	// Trigger the Primary Attack Animation.
	void PrimaryAttack();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
