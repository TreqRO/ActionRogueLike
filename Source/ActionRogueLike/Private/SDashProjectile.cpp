// Fill out your copyright notice in the Description page of Project Settings.


#include "SDashProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
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

void ASDashProjectile::TeleportInstigatorAfterProjectileHit()
{
	FRotator InstigatorRotation = InstigatorCharacter->GetActorRotation();
	InstigatorCharacter->TeleportTo(LocationProjectile, InstigatorRotation);
	Destroy();
}

void ASDashProjectile::OnActorHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	UE_LOG(LogTemp, Log, TEXT("OnActorHit in Dash Projectile"));
	ProjectileTeleportAfterTimeElapsed();

	//UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionParticle, LocationProjectile, RotationProjectile);
	//Binding the function with specific values
	//TimerDel.BindUFunction(this, FName("TeleportInstigator"), InstigatorCharacter, LocationProjectile, RotationProjectile);
	//InstigatorCharacter->TeleportTo(LocationProjectile, RotationProjectile);
	//GetWorldTimerManager().SetTimer(TimerHandle_Projectile, TimerDel, 0.2f, false);
	//GetWorld()->GetTimerManager().SetTimer(TimerHandle_Projectile,[&]() { this->TeleportInstigator(InstigatorCharacter, LocationProjectile, RotationProjectile); },0.2f, false);
	//GetWorldTimerManager().SetTimer(TimerHandle_Projectile, this, &ASDashProjectile::TeleportInstigatorAfterProjectileHit, 0.2f);

	//GetWorldTimerManager().SetTimer(TimerHandle_Projectile, 0.2f);

	//GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.2f);

}

void ASDashProjectile::ProjectileTeleport()
{
	UE_LOG(LogTemp, Log, TEXT("ProjectileTeleport in Dash Projectile"));
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

