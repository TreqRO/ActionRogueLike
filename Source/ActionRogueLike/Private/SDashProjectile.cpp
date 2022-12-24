// Fill out your copyright notice in the Description page of Project Settings.


#include "SDashProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
ASDashProjectile::ASDashProjectile()
{
	//MOVED IN THE SHARED CLASS
	TeleportDelay = 0.2f;
	DetonateDelay = 0.2f;

	MovementComp->InitialSpeed = 6000.0f;
	
}

void ASDashProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_DelayedDetonate, this, &ASDashProjectile::Explode, DetonateDelay);
}

void ASDashProjectile::Explode_Implementation()
{
	// Clear timer if the Explode was already called through another source like OnActorHit
	GetWorldTimerManager().ClearTimer(TimerHandle_DelayedDetonate);

	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());

	EffectComp->DeactivateSystem();

	MovementComp->StopMovementImmediately();
	SetActorEnableCollision(false);

	// Set as local variable since we don't have ever an intention of canceling that event.
	FTimerHandle TimeHandle_DelayedTeleport;
	GetWorldTimerManager().SetTimer(TimeHandle_DelayedTeleport, this, &ASDashProjectile::TeleportInstigator, TeleportDelay);

	//Skip base implementation as it will destroy actor (we need to stay alive a bit longer to finish the 2nd timer aka the Teleport delay)
	//Super::Explode_Implementation();

}


void ASDashProjectile::TeleportInstigator()
{
	AActor* ActorToTeleport = GetInstigator();
	if(ensure(ActorToTeleport))
	{
		//Keep instigator rotation or it may end up jarring
		ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation(), false, false);
	}

	Destroy();
}



	/*
	 * MY WAY
	 */

/*
void ASDashProjectile::TeleportInstigatorAfterProjectileHit()
{
	FRotator InstigatorRotation = InstigatorCharacter->GetActorRotation();
	InstigatorCharacter->TeleportTo(LocationProjectile, InstigatorRotation);
	Destroy();
}

void ASDashProjectile::OnActorHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	//UE_LOG(LogTemp, Log, TEXT("OnActorHit in Dash Projectile"));
	ProjectileTeleportAfterTimeElapsed();
}

void ASDashProjectile::ProjectileTeleport()
{
	//UE_LOG(LogTemp, Log, TEXT("ProjectileTeleport in Dash Projectile"));
	GetWorldTimerManager().SetTimer(TimerHandle_Projectile, this, &ASDashProjectile::ProjectileTeleportAfterTimeElapsed, 0.2f);
}

void ASDashProjectile::ProjectileTeleportAfterTimeElapsed()
{
	MovementComp->StopMovementImmediately();
	InstigatorCharacter = GetInstigator();
	RotationProjectile = SphereComp->GetComponentRotation();
	LocationProjectile = SphereComp->GetComponentLocation();
	EffectComp->DestroyComponent();
	UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionParticle, LocationProjectile, RotationProjectile);
	GetWorldTimerManager().SetTimer(TimerHandle_Projectile, this, &ASDashProjectile::TeleportInstigatorAfterProjectileHit, 0.2f);

}


void ASDashProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComp->OnComponentHit.AddDynamic(this, &ASDashProjectile::OnActorHit);

}

// Called when the game starts or when spawned
void ASDashProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	ProjectileTeleport();
}

// Called every frame
void ASDashProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


*/
