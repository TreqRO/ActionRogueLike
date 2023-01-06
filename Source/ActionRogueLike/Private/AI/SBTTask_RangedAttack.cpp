// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_RangedAttack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type USBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// The goal of the task is to spawn a projectile in the direction of our target actor
	AAIController* MyController = OwnerComp.GetAIOwner();

	if (ensure(MyController))
	{
		// From my controller we can get the pawn and we cast it as character
		// We do this because we want to get the Muzzle location (the handsocket bone name) from this character.  
		ACharacter* MyPawn = Cast<ACharacter>(MyController->GetPawn());

		if(MyPawn == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		// The character has a GetMesh class available to us
		FVector MuzzleLocation = MyPawn->GetMesh()->GetSocketLocation("Muzzle_01");

		// Get the target actor
		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));

		if (TargetActor == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		// Gives the direction vector pointing toward the target Actor (target - origin)
		FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
		FRotator MuzzleRotation = Direction.Rotation();

		//Spawn the projectile
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// Expose the Projectile Class in the BT BP 
		AActor* NewProj = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, Params);

		// Check if the projectile had spawn successfully
		return NewProj ? EBTNodeResult::Succeeded : EBTNodeResult::Failed; 

	}
	return EBTNodeResult::Failed;
	//return Super::ExecuteTask(OwnerComp, NodeMemory);
}
