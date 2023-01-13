// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_Heal.h"
#include "AIController.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type USBTTask_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//return Super::ExecuteTask(OwnerComp, NodeMemory);
	// The goal of the task is to fully heal the "Injured" AI.

	APawn* MyPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if(MyPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(MyPawn);

	if(ensure(AttributeComp))
	{
		AttributeComp->ApplyHealthChange(MyPawn, AttributeComp->GetHealthMax());
	}

	return EBTNodeResult::Succeeded;

}
	// MY WAY
//	AAIController* MyController = OwnerComp.GetAIOwner();
//
//	if(ensure(MyController))
//	{
//		// Get the AI Pawn
//		APawn* MyPawn = MyController->GetPawn();
//
//		if(MyPawn)
//		{
//			// Get the attribute component of the AI Pawn
//			USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(MyPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
//
//			// Fully heal the AI to Max HP.
//			if(ensure(AttributeComp))
//			{
//				// Heal AI
//				AttributeComp->ApplyHealthChange(AttributeComp->GetHealthMax());
//
//				// Set the new value of AICurrentHealth to the Blackboard.
//				UBlackboardComponent* BBComp = MyController->GetBlackboardComponent();
//				BBComp->SetValueAsFloat("AICurrentHealth", AttributeComp->GetHealthMax());
//
//				if(BBComp->GetValueAsFloat("AICurrentHealth") > 0.0f)
//				{
//					return EBTNodeResult::Succeeded;
//				}
//				return EBTNodeResult::Failed;
//			}
//		}
//	}
//	return EBTNodeResult::Failed;
//}
