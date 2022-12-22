// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSharedProjectileClass.h"
#include "SDashProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;

UCLASS()
class ASDashProjectile : public ASSharedProjectileClass
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASDashProjectile();
	

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float TeleportDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float DetonateDelay;

	//Handle to cancel timer if we already hit something
	FTimerHandle TimerHandle_DelayedDetonate;

	// Base class using BlueprintNativeEvent, we must override the _Implementation not the Explode()
	virtual void Explode_Implementation() override;

	void TeleportInstigator();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
	/*
	* MY WAY
	*/

	/*
	void TeleportInstigatorAfterProjectileHit();

	void ProjectileTeleport();
	void ProjectileTeleportAfterTimeElapsed();

	//virtual void PostInitializeComponents() override;
	FTimerHandle TimerHandle_Projectile;
	FTimerDelegate TimerDel;
	AActor* InstigatorCharacter;
	FRotator RotationProjectile;
	FVector LocationProjectile;

	// Must be marked with ufunction in order to 'bind' the event
	UFUNCTION()
	void OnActorHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere, Category = "Attack")
	UParticleSystem* ExplosionParticle;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

	*/