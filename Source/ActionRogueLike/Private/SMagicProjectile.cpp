// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Make sure that all the components have an instance
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	// Access the sphere component collision (Best to set a profile name from the editor or create a custom one)

	// Below a viable option however the best option would be to create a new Profile
	// Set the 'Object'type (in this case this is a WorldDynamic)
	//SphereComp->SetCollisionObjectType(ECC_WorldDynamic);
	//Can set all the collision channels to be ignored and then
	//SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	//just overlapping with the pawn
	//SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	// Edit->Project Setting->Engine->Collision->Preset->New (add the the same name as it is in the editor)
	SphereComp->SetCollisionProfileName("Projectile");

	RootComponent = SphereComp;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	// Attach the effect component to the Sphere component
	EffectComp->SetupAttachment(SphereComp);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	//Setup some movement of the projectile.
	MovementComp->InitialSpeed = 1000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
}

// Called when the game starts or when spawned
void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

