// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SGameplayInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USGameplayInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ACTIONROGUELIKE_API ISGameplayInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//We want to implement this not only in c++ but also in blueprint hence we need to expose this function to the blueprints.
	//BlueprintImplementationEvent: ONLY ALLOWS YOU TO HAVE AN IMPLEMENTATION IN BLUEPRINT NOT C++ Even though the baseclass is implemented in c++.
	//BlueprintNativeEvent: IMPLEMENTATION ON BOTH BLUEPRINT AND C++ ALLOWED
	//BluePrintCallable allows you to call it in BluePrint
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	// Because we want to have some sort of context of who was using/trigeering it(the health pot), we pass along our player which would be
	// the instigator pawn. TLDR: We know who triggered the health position hence to whom we should apply the effect. 
	void Interact(APawn* InstigatorPawn);
};
