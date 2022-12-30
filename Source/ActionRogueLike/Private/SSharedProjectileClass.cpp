// Fill out your copyright notice in the Description page of Project Settings.


#include "SSharedProjectileClass.h"

#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASSharedProjectileClass::ASSharedProjectileClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	//SphereComp->SetCollisionProfileName("Projectile")
	RootComponent = SphereComp;

	// Access the sphere component collision (Best to set a profile name from the editor or create a custom one)
	// Below a viable option however the best option would be to create a new Profile
	// Set the 'Object'type (in this case this is a WorldDynamic)
	//SphereComp->SetCollisionObjectType(ECC_WorldDynamic);
	//Can set all the collision channels to be ignored and then
	//SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	//just overlapping with the pawn
	//SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	// Edit->Project Setting->Engine->Collision->Preset->New (add the the same name as it is in the editor)
	//SphereComp->SetCollisionProfileName("Projectile")

	
	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(RootComponent);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 8000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
	MovementComp->ProjectileGravityScale = 0.0f;

	AudioComp = CreateDefaultSubobject<UAudioComponent>("AudioComp");
	AudioComp->SetupAttachment(RootComponent);
}

void ASSharedProjectileClass::OnActorHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}

// _Implementation from it being marked as BlueprintNativeEvent
void ASSharedProjectileClass::Explode_Implementation()
{
	// Check to make sure we aren't already being 'destroyed'
	// Adding ensure to see if we encounter this situation at all
	if (ensure(IsValid(this)))
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());

		//Impact Sound
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), GetActorRotation());

		Destroy();
	}
}




// Called when the game starts or when spawned
void ASSharedProjectileClass::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);

	// More consistent to bind here compared to Constructor which may fail to bind if Blueprint was created before adding this binding (or when using hotreload)
	// PostInitializeComponent is the preferred way of binding any events.
	SphereComp->OnComponentHit.AddDynamic(this, &ASSharedProjectileClass::OnActorHit);
}

