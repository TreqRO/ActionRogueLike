// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckIfLowHealth.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_CheckIfLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                            float DeltaSeconds)
{
    //Make sure to call super since we don't want to override any base logic.
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    // GOAL: Check periodically if the AI Health is low (below 40%)

    //Pass by reference(&) use dot(.) by pointer is ->
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

    if ensure((BlackboardComp))
    {
        // Get the current health from the blackboard
        float CurrentHealth = BlackboardComp->GetValueAsFloat("AICurrentHealth");

        bool bIsLowHealth = CurrentHealth <= 40.0f;
    	BlackboardComp->SetValueAsBool(AILowHealthKey.SelectedKeyName, bIsLowHealth);
    }
}
