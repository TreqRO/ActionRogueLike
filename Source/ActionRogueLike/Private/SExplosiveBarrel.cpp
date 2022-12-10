// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Make the instance of the Mesh component
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
	// The object would drop to the floor and would be influenced by its own force application from the force component
	StaticMeshComp->SetSimulatePhysics(true);
	//Make the root component as the MeshComp
	RootComponent = StaticMeshComp;

	//Make the instance of the Radial Force (the BOOM)
	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");
	RadialForceComp->SetupAttachment(StaticMeshComp);

	//Leaving this on applies constant force via component 'tick'
	RadialForceComp->SetAutoActivate(false);

	//Setup the big Boom when the barrel is hit with the projectile
	//How much will the explosion affect other objects. (push back)
	//cat de tare te arunca in aer
	RadialForceComp->ImpulseStrength = 2500.0f;
	// Cat de aproape trebuie sa fii de obiect ca sa explodeze
	RadialForceComp->Radius = 750.0f;
	// Ignores 'Mass' of others objects (if false, the impulse strength will be much higher to push objects depending on Mass)
	RadialForceComp->bImpulseVelChange = true;

	// Default constructor of component already adds 4 object types to affect, excluding WorldDynamic
	RadialForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);

	// Binding either in constructor or in Begin Play or in PostInitializeComponents() (the general advice function to put it in) 
	// StaticMeshComp->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnActorHit);
}

void ASExplosiveBarrel::OnActorHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	RadialForceComp->FireImpulse();
}


// It start before Begin Play (runs between the construct and the begin play, in case we do other stuff in begin play)
void ASExplosiveBarrel::PostInitializeComponents()
{
	// Don't forget to call parent function
	Super::PostInitializeComponents();
	StaticMeshComp->OnComponentHit.AddDynamic(this,&ASExplosiveBarrel::OnActorHit);
	
}

// Called every frame
void ASExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

