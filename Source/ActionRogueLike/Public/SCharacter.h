// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"


class USAttributeComponent;
class UCameraComponent;
class USpringArmComponent;
class USInteractionComponent;
class UAnimMontage;
class UParticleSystem;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

	// VisibleAnywhere = read-only, still useful to view in-editor and enforce a convention.
	UPROPERTY(VisibleAnywhere, Category="Effects")
	FName TimeToHitParamName;

	UPROPERTY(VisibleAnywhere, Category="Effects")
	FName HandSocketName;

	// Allows us to assign a class (it could be an actor, pawn, character or in this case our Magic Projectile because Magic Projectile
	// derives from actor.
	// Make sure to add an UPROPERTY to make sure that this is visible from the editor and that we can actually assign a variable.
	UPROPERTY(EditAnywhere, Category = "Attack")
	// MAKE SURE TO ASSIGN THIS IN BLUEPRINT (Click on the PlayerCharacter BluePrint and search for "ProjectileClass" on the write side and assign the MagicProjectileBP (blue print))
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileBlackHoleClass;

	UPROPERTY(EditAnywhere, Category = "Dash")
	TSubclassOf<AActor> ProjectileDashClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	// The Attack Animation WHICH IS PASSED IN THE EDITOR!
	UAnimMontage* AttackAnim;

	// Particle System played during attack animation
	UPROPERTY(EditAnywhere, Category="Attack")
	UParticleSystem* CastingEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float AttackAnimDelay;

	FTimerHandle TimerHandle_PrimaryAttack;
	FTimerHandle TimerHandle_Dash;
	FTimerHandle TimerHandle_BlackHoleAttack;


public:
	// Sets default values for this character's properties
	ASCharacter();

protected:

	//Allows you the see all it's properties in the Engine.
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp; // 3rd person character "Spring" component

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComp;

	// Put in the category "Components" all your components (standard convention)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USAttributeComponent* AttributeComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Moves the character forward/backwards. Values from -1 to 0 and from 0 to 1 (Image a joystick)
	void MoveForward(float Value);

	// Move the character left/right
	void MoveRight(float Value);

	// Trigger the Primary Attack Animation.
	void PrimaryAttack();

	// Trigger attack after delay
	void PrimaryAttack_TimeElapsed();

	//Trigger the Primary Interact
	void PrimaryInteract();

	//Trigger the BlackHoleAttack
	void BlackholeAttack();

	//Trigger the BlackHoleAttack after delay.
	void BlackholeAttack_TimeElapsed();

	void Dash();

	void Dash_TimeElapsed();

	void StartAttackEffects();

	// Re-use spawn logic between attacks
	void SpawnProjectile(TSubclassOf<AActor> ClassToSpawn);

	// Don't forget to bind the Function!
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

	// Best way to bind your events!
	virtual void PostInitializeComponents() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
};
