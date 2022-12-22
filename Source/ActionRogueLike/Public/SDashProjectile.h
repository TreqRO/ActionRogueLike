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
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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
