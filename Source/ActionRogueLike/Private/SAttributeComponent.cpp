// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	Health = 100;
}


bool USAttributeComponent::ApplyHealthChange(float Delta)
{
	// Here we make the actual change to our Health variable rather than giving direct access to it. 
	Health += Delta;
	// Trigger the event whenever our health has been changed
	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
	//Since we don't have any success or fail right now, we just return true.
	return true;
}




