// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	HealthMax = 100;
	Health = HealthMax;


}


bool USAttributeComponent::IsFullHealth() const
{
	return Health == HealthMax;
}

float USAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}



bool USAttributeComponent::ApplyHealthChange(float Delta)
{
	// Here we make the actual change to our Health variable rather than giving direct access to it. 
	//Health += Delta;
	float HealthOld = Health;

	// Clamp the Health Attribute between 0 and HealthMax via ApplyHealthChange
	Health = FMath::Clamp(Health + Delta, 0, HealthMax);

	float ActualDelta = Health - HealthOld;
		
	// Trigger the event whenever our health has been changed
	OnHealthChanged.Broadcast(nullptr, this, Health, ActualDelta);
	//Since we don't have any success or fail right now, we just return true.
	return ActualDelta!= 0;
}
// By making it constant it is basically a get function. It's has read only access to this information. (rather thant full read write access basically)
bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}


