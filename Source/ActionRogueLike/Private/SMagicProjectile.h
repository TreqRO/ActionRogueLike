// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSharedProjectileClass.h"
#include "GameFramework/Actor.h"
#include "SMagicProjectile.generated.h"

//MOVED INTO THE THE SHARED PROJECTILE CLASS (SSharedProjectileClass)
//class USphereComponent;
//class UProjectileMovementComponent;
//class UParticleSystemComponent;

UCLASS()
class ASMagicProjectile : public ASSharedProjectileClass
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASMagicProjectile();

protected:

	//MOVED INTO THE THE SHARED PROJECTILE CLASS (SSharedProjectileClass)
	//// The collision component
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//USphereComponent* SphereComp;

	//// A component which we give a velocity and it will go in a straight line.
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//UProjectileMovementComponent* MovementComp;

	//// Particle system component just so that we have something to show in the world.
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//UParticleSystemComponent* EffectComp;

	// We are unable to properly bind to this function without UFUNCTION
	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Damage");
	float DamageAmount;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
