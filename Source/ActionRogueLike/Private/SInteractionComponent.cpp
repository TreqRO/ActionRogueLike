// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"

#include "SGameplayInterface.h"


// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USInteractionComponent::PrimaryInteract()
{
	// Basically the interaction between the actor and the chest using collision queries. The Character looks at the box, presses E and then the box open

	//We do the collision queries just like spawning of Actor, we do it through Get World.

	// Line trace object we just want to find the first object that it finds. 
	// Basically what the line trace does is that it will start a certain point in the world and it will go to a certain point in the world and it will try
	// to figure out if there is some kind of object to collide with in between.
	// We want to find anything of type World Dynamic (the chest will be World Dynamic)

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	// Since this component is attached to our character we have a very easy way to get to that character and that is simply to get the owner,
	// When you call the owner in the context of a component you will get whoever we are attached to.
	AActor* MyOwner = GetOwner();

	// Fills the location and the rotation of the actor
	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	// Start position currently the actor eye location (not camera because we haven't properly fixed that yet
	// End position EyeLocation + extend in the direction we are looking  * 1000 cm
	FVector End = EyeLocation + (EyeRotation.Vector() * 500);

	// Fills the hit information
	//FHitResult Hit;
	// Hit is basically which object is hit (pressed)
	// bBlockingHit checks if it had hit anything
	//bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);

	TArray<FHitResult>Hits;
	// Basically checks if it had hit something in the SIZE OF YOUR BEAM (BEFORE THE LineTrace was basically pixel based)
	//FQuat is basically a better version of Rotator that has a little bit more information
	// The sphere doesn't need to rotate hence Identity matrix for rotation
	FCollisionShape Shape;

	float Radius = 60.0f;

	Shape.SetSphere(Radius);
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);

	// If it hit something then the color of the line is green, otherwise is red.
	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	for(FHitResult Hit: Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			// Check if the Actor implements the Gameplay Interface (if so it should be the SItemChest)
			// Make sure to use the USGameplayInterface here!! not IS
			// Use the U to check if we implemented that one but whenever we call on it it is I!!!!
			if (HitActor->Implements<USGameplayInterface>())
			{
				//Execute_NameOfTheFunctionInCPP
				//First parameter is whatever object we want to call this function on (in this case the HitActor)
				// The instigator pawn is the character (the owner) of which we as a component are a part of.
				//Cast the owner(which is actor) to a pawn in order to use it for the Execute_Interact function
				APawn* MyPawn = Cast<APawn>(MyOwner);

				ISGameplayInterface::Execute_Interact(HitActor, MyPawn);

				// If two chest are one next to each other and sphere beam is on both, then it only opens the first POINT belonging to that chest
				break;
			}
		}
		// Shows how big that sphere is that we're actually sweeping through the world
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, LineColor, false, 2.0f);
	}

	
	//(DRAWS STRAIGHT A LINE FROM EYES TO END POINT)
	DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 2.0f);



}