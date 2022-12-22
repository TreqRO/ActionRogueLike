// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SSharedProjectileClass.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;

UCLASS(ABSTRACT) //'ABSTRACT' marks this class as incomplete, keeping this out of certain dropdowns windows likes
//SpawnActor in Unreal Engine
class ACTIONROGUELIKE_API ASSharedProjectileClass : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASSharedProjectileClass();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* ImpactVFX;

	// The collision component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USphereComponent* SphereComp;

	// A component which we give a velocity and it will go in a straight line.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* MovementComp;

	// Particle system component just so that we have something to show in the world.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* EffectComp;

	//'virtual' so we can override this in child-classes 
	// Must be marked with ufunction in order to 'bind' the event
	UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	//BlueprintNativeEvent = C++ base implementation, can be expanded in Blueprints
	//BlueprintCallable to allow child classes to trigger explosions
	//Not required for assignment, useful for expanding in Blueprint later on.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Explode();

	virtual void PostInitializeComponents() override;



};
