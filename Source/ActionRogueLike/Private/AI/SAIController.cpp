// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

// The Brains of our AI Character

void ASAIController::BeginPlay()
{
	// Call super in order not lose any of the default logic.
	Super::BeginPlay();
	RunBehaviorTree(BehaviorTree);

	// Placeholder Code
	// Going to grab Player Character and I'm just going to add his location into here. (usually use the PawnSensing)
	// WorldContextObject - pass any type of object that lives in the world. This will be good enough because it lives in the world.
	// PlayerIndex: 0 (because we just want the player we are using. (player index is used if you have split screen for example))
	APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	// Making sure it is not null
	if(MyPawn)
	{
		// Get the MinionRanged Blackboard
		// MoveToLocation vector added inside the BB (using the Editor)
		// The AI moves to the Spawn Location of the Player Character
		GetBlackboardComponent()->SetValueAsVector("MoveToLocation", MyPawn->GetActorLocation());

		// TargetActor "actor "object added inside BB
		// We are passing in our Player Character
		GetBlackboardComponent()->SetValueAsObject("TargetActor", MyPawn);
	}

}