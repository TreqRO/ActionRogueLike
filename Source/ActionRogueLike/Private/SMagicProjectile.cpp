// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"
#include "SAttributeComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
	//MOVED IN THE SHARED CLASS
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);

	DamageAmount = 20.0f;

	//CameraShakeBase = CreateDefaultSubobject<UCameraShakeBase>(TEXT("CameraShakeBase"));
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	

	// Check if the other actor that we hit is valid (is not a null pointer), otherwise we will crash on it once we try to call functions on the other actor).
	if(OtherActor && OtherActor != GetInstigator())
	{
		// Then we need to get the component, the Attribute Components, and then we want to call the apply health change function on it.
		// Using StaticClass we can pass around the class that we require. 
		// The GetCompnentByClass returns only type UActorComponent so we have to cast it to USAttributeComponent
		USAttributeComponent* AttributeComp =Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));

		// Can still be null if we hit a wall or we hit some kind of cube, that doesn't have the attribute component
		if (AttributeComp)
		{


			// Add the camera shake
			//UGameplayStatics::PlayWorldCameraShake(this, ImpactShake, SweepResult.ImpactPoint, 1000.0f, 1000.0f);
			UGameplayStatics::PlayWorldCameraShake(this, ImpactShake, GetActorLocation(), ImpactShakeInnerRadius, ImpactShakeOuterRadius);

			//Impact Sound
			UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), GetActorRotation());

			// Now if we hit whoever has the attribute (it doesn't have to be the character, it can be also the explosive barrel), the health
			// will be reduce by 20. 
			AttributeComp->ApplyHealthChange(this, -DamageAmount);
			Destroy();
		}
	}
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

