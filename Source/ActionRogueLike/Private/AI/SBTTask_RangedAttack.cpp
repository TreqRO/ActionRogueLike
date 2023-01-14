// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_RangedAttack.h"
#include "AIController.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"



USBTTask_RangedAttack::USBTTask_RangedAttack()
{
	// This will be in degrees (pith and yawn work in degrees)
	MaxBulletSpread = 2.0f;
}

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

		// If the Target Actor is not alive, then we return failed.
		// This will not only end the task before we even spawn the actor but also basically fail this entire tree on the BT (It will exist the loop).
		// It will still come back in so we must make sure that we clear the set target actor somewhere else.
		if (!USAttributeComponent::IsActorAlive(TargetActor))
		{
			return EBTNodeResult::Failed;
		}

		// Gives the direction vector pointing toward the target Actor (target - origin)
		FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
		FRotator MuzzleRotation = Direction.Rotation();

		// We want to make sure that we add a little bit of offset (a little bit of random weapon spread) because the guys are currently just laser targeting us and they are way too good
		// at shooting (random number are huge part of making games).

		//Adjust the muzzle rotation in pitch (up and down) and yawn (left and right)
		// I don't want him to ever shoot downward (hence Pith min offset 0)
		MuzzleRotation.Pitch += FMath::RandRange(0.0f, MaxBulletSpread);
		MuzzleRotation.Yaw += FMath::RandRange(-MaxBulletSpread, MaxBulletSpread);

		//Spawn the projectile
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		// If we don't specify the instigator we can actually overlap with ourselves! (hence hitting himself)
		SpawnParams.Instigator = MyPawn;


		// Expose the Projectile Class in the BT BP 
		AActor* NewProj = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);

		// Check if the projectile had spawn successfully
		return NewProj ? EBTNodeResult::Succeeded : EBTNodeResult::Failed; 

	}
	return EBTNodeResult::Failed;
	//return Super::ExecuteTask(OwnerComp, NodeMemory);
}

