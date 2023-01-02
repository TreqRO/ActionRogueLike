// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerup_HealthPotion.h"
#include "SAttributeComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ASPowerup_HealthPotion::ASPowerup_HealthPotion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	//SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	//RootComponent = SphereComp;
	//SphereComp->SetSphereRadius(50);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");

	// Disable collision, instead we use SphereComp to handle interaction queries
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MeshComp->SetupAttachment(RootComponent);
}


void ASPowerup_HealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if(!ensure(InstigatorPawn))
	{
		return;
	}

	USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));

	// Check if not already at max health
	if (ensure(AttributeComp) && !AttributeComp->IsFullHealth())
	{
		// Only activate if healed successfully
		// Full Health the player. 
		if (AttributeComp->ApplyHealthChange(AttributeComp->GetHealthMax()))
		{
			HideAndCooldownPowerup();
		}
	}
}





/* MY WAY
void ASPowerup_HealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	//ISGameplayInterface::Interact_Implementation(InstigatorPawn);
	if (IsValid(InstigatorPawn)){
		// Then we need to get the component, the Attribute Components, and then we want to call the apply health change function on it.
		// Using StaticClass we can pass around the class that we require. 
		// The GetCompnentByClass returns only type UActorComponent so we have to cast it to USAttributeComponent
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));;

		if(AttributeComp)
		{
			if ((!AttributeComp->IsFullHealth()) && (AttributeComp->IsAlive()) && (PlayerInteracted == false))
			{
				PlayerInteracted = true;
				bool health_applied = AttributeComp->ApplyHealthChange(20);
				MeshComp->SetVisibility(false);

				// Add a timer so the Projectile will spawn when the hand is fully extended during the animation
				FTimerHandle TimeHandle_Interaction;
				GetWorldTimerManager().SetTimer(TimeHandle_Interaction, this, &ASPowerup_HealthPotion::UsableAgain, 5);
			}
		}
	}
}

void ASPowerup_HealthPotion::UsableAgain()
{
	MeshComp->SetVisibility(true);
	PlayerInteracted = false;
}

// Called when the game starts or when spawned
void ASPowerup_HealthPotion::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASPowerup_HealthPotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

*/