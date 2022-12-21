// Fill out your copyright notice in the Description page of Project Settings.


#include "SDashProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASDashProjectile::ASDashProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	RootComponent = SphereComp;
	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 1000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
}

void ASDashProjectile::OnActorHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//OtherActor->TeleportTo()
	AActor* InstigatorCharacter = GetInstigator();
	FRotator RotationProjectile = SphereComp->GetComponentRotation();
	FVector LocationProjectile = SphereComp->GetComponentLocation();
	InstigatorCharacter->TeleportTo(LocationProjectile, RotationProjectile);

	//Instigator->TeleportTo()
	// Add a timer so the Projectile will spawn when the hand is fully extended during the animation
	//GetWorldTimerManager().SetTimer(TimerHandle_Projectile, this, &ASDashProjectile::Dash_TimeElapsed, 0.2f);
}


// Called when the game starts or when spawned
void ASDashProjectile::BeginPlay()
{
	Super::BeginPlay();
	SphereComp->OnComponentHit.AddDynamic(this, &ASDashProjectile::OnActorHit);
	
}

// Called every frame
void ASDashProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

