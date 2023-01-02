// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerupActor.h"
#include "SPowerup_HealthPotion.generated.h"

//class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class ACTIONROGUELIKE_API ASPowerup_HealthPotion : public ASPowerupActor
{
	GENERATED_BODY()

protected:

	// The collision component
	//UPROPERTY(VisibleAnywhere, Category = "Components")
	//USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;

	//bool PlayerInteracted = false;

	//float health amount?

	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;
	
public:	
	void Interact_Implementation(APawn* InstigatorPawn) override;

	// Sets default values for this actor's properties
	ASPowerup_HealthPotion();

	//void UsableAgain();



};
