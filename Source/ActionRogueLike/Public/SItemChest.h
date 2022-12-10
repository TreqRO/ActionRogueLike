// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SItemChest.generated.h"

// derive also from ISGameplayInterface and then we must implement the VIRTUAL METHODS
UCLASS()
class ACTIONROGUELIKE_API ASItemChest : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()

	//implement the VIRTUAL METHODS
	// The _Implementation is added to the name because we specified our Blueprint Native Event
	// without it it would be an "Interact"
	void Interact_Implementation(APawn* InstigatorPawn);

public:

	// We are able to tweek the Opening of the chess value in the blueprint if we are using the UPROPERTY(EditAnywhere)
	UPROPERTY(EditAnywhere)
	float TargetPitch;


protected:
	//Pirate chess static mesh components.

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	// BlueprintReadOnly allows you to use it on the event graph (blueprint to animate the opening of the lid)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly);
	UStaticMeshComponent* LidMesh;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Sets default values for this actor's properties
	ASItemChest();
};
