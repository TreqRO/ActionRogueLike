// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SBTService_CheckIfLowHealth.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USBTService_CheckIfLowHealth : public UBTService
{
	GENERATED_BODY()
protected:

	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector AILowHealthKey;

	//Threshold that is considered 'low health' (e.g. 0.3 is 30% of maximum health)
	UPROPERTY(EditAnywhere, Category = "AI", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float LowHealthFraction;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	USBTService_CheckIfLowHealth();
};
