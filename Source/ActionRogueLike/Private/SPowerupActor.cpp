// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerupActor.h"

#include "Components/SphereComponent.h"

// Sets default values
ASPowerupActor::ASPowerupActor()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");

	SphereComp->SetCollisionProfileName("Powerup");

	RootComponent = SphereComp;

	RespawnTime = 10.0f;

}

void ASPowerupActor::Interact_Implementation(APawn* InstigatorPawn)
{
	//logic in derived classes...
}

void ASPowerupActor::SetPowerupState(bool bNewIsActive)
{
	SetActorEnableCollision(bNewIsActive);

	//Set visibility on root and all children
	RootComponent->SetVisibility(bNewIsActive, true);
}

void ASPowerupActor::ShowPowerUp()
{
	SetPowerupState(true);
}

void ASPowerupActor::HideAndCooldownPowerup()
{
	SetPowerupState(false);

	FTimerHandle TimeHandle_RespawnTimer;

	GetWorldTimerManager().SetTimer(TimeHandle_RespawnTimer, this, &ASPowerupActor::ShowPowerUp, RespawnTime);
}







