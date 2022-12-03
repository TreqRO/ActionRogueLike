// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Make the instance of the Mesh component
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");

	//Make the instance of the Radial Force (the BOOM)
	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");

	//Make the root component as the MeshComp
	RootComponent = StaticMeshComp;
	RadialForceComp->SetupAttachment(StaticMeshComp);

	//RadialForceComp->FireImpulse();
	//Setup the big Boom when the barrel is hit with the projectile
	//How much will the explosion affect other objects. (push back)
	//cat de tare te arunca in aer
	RadialForceComp->ImpulseStrength = 2000.0f;
	// Cat de aproape trebuie sa fii de obiect ca sa explodeze
	RadialForceComp->Radius = 700.0;
	RadialForceComp->bImpulseVelChange = true;
}

void ASExplosiveBarrel::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	RadialForceComp->FireImpulse();
}


// Called when the game starts or when spawned
void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	StaticMeshComp->OnComponentHit.AddDynamic(this,&ASExplosiveBarrel::OnHit);
	
}

// Called every frame
void ASExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

