// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

// Making our own event, Health Event
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, USAttributeComponent*, OwningComp, float, NewHealth, float, Delta);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USAttributeComponent();

protected:
	// EditAnywhere - edit in BP and per-instance in level
	// VisibleAnywhere - 'read-only' in editor and level (use for Components)
	// EditDefaultOnly - hide variable per-instance, edit in BP editor only (once the player character had been put in the level, we can't change it in the details panel anymore.
	// VisibleDefaultsOnly - 'read-only' access to variable, only in BP editor (uncommon)
	// --
	// BlueprintReadOnly - read-only in the Blueprint scripting (doesn't not affect 'details' - panel)
	// BlueprintReadWrite - read-write access in Blueprints
	//--
	// Category = "" - display only for detail panels and blueprint context menu.

	// put it under protected since I don't want other classes to give direct access to it.
	// We are going to make a function that gives access to changes to this variable (the ApplyHealthChange)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float Health;

	//HealthMax, Stamina, Strength

public:	

	// Allows us in BP to bind to this event
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	// We want to deal some kind of damage or some healing using delta which will be the difference that we are going to apply to the health
	// We want BP to call this
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	//bool so we know whether or not the change actually succeeded (in case we are invincible or we already died)
	bool ApplyHealthChange(float Delta);
		
};