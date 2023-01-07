// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class UPawnSensingComponent;
class USAttributeComponent;

UCLASS()
class ACTIONROGUELIKE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	// VisibleAnywhere = read-only, still useful to view in-editor and enforce a convention.
	//UPROPERTY(VisibleAnywhere, Category = "Effects")
	//FName TimeToHitParamName;

	//UPROPERTY(EditDefaultsOnly, Category = "AI")
	//// Which in BP is named WithinAttackRange
	//FBlackboardKeySelector AICurrentHealth;

public:
	// Sets default values for this character's properties
	ASAICharacter();

protected:

	// Sight AI
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	// Attributes (Health, HealthMax)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USAttributeComponent* AttributeComp;

	// Don't forget to bind the Function!
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	virtual void PostInitializeComponents() override;
};
