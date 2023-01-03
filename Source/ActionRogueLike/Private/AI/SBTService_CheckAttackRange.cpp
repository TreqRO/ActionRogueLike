// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckAttackRange.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                            float DeltaSeconds)
{
	// THE SERVICE RUNS IN THE BACKGROUND BY DEFAULT EVERY 0.5 SECONDS  Check distance between AI pawn and Target Actor

	//Make sure to call super since we don't want to override any base logic.
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// GOAL : Check distance between AI pawn and Target Actor

	//Pass by reference(&) use dot(.) by pointer is ->
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if ensure((BlackboardComp))
	{
		// Get our Target Actor from the blackboard (cast it as AActor)
		AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor"));

		//Make sure it's not null 
		if (TargetActor)
		{
			// Get the AI Controller (from which we can then get our the AI Pawn)
			AAIController* MyController =  OwnerComp.GetAIOwner();

			if(ensure(MyController))
			{
				// Get the AI Pawn
				APawn* AIPawn = MyController->GetPawn();
				// As soon as our AI character is dead or it's gone we should definitely stopping our behavior tree. 
				if(ensure(AIPawn))
				{
					// Get the distance number between the TargetActor (Player) and the AI.
					float DistanceTo = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());

					// if it's less than 2000 then we are within range. 
					bool bWithinRange = DistanceTo < 2000.0f;
					bool bHasLOS = false;

					// A bit more efficient, only check if in sight if you are range otherwise don't both
					if(bWithinRange)
					{
						// Check if the Actor is in the Line of Sight of the AI (e.g in cases where there are walls between the AI and the player)
						// uses ECC_Visibility
						bHasLOS = MyController->LineOfSightTo(TargetActor);
					}

					// put this data back into our blackboard, because the blackboard is suppose to be the memory of our AI
					// KeyNamed WithinAttackRange in BP
					// We are then using this information in the Behavior Tree to check whether or not we are range and should cancel or continue to move towards target
					BlackboardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, (bWithinRange && bHasLOS));
				}
			}
		}
	}
}
