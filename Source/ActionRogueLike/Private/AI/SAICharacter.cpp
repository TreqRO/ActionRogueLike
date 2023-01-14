// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
ASAICharacter::ASAICharacter()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	// Put the AI Behavior tree on a minion that is either placed or spawned in the world.
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}


// Not Used anymore
void ASAICharacter::SetInBBInitialHealth()
{
	AAIController* AIC = Cast<AAIController>(GetController());

	if (AIC)
	{
		UBlackboardComponent* BBComp = AIC->GetBlackboardComponent();

		if(BBComp)
		{
			float CurrentHealth = AttributeComp->GetCurrentHealth();
			BBComp->SetValueAsFloat("AICurrentHealth", CurrentHealth);

		}


		// The name set on BP
		// We pass the game time which is synchronous with that time node in our material 
		//GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
	}
}

void ASAICharacter::BeginPlay()
{
	Super::BeginPlay();
	//SetInBBInitialHealth();
}


void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();



	PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);

	// Bind the OnHealthChange function!
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);

	


}

void ASAICharacter::OnPawnSeen(APawn* Pawn)
{

	SetTargetActor(Pawn);
	// At the location of the actor draw a debug string so that we have something where the play has been spotted 
	DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);
}

void ASAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);;
	}

}

void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	
	if (Delta < 0.0f)
	{

		// Allow the AI to react to damage by setting the Target Actor.
		//Whenever the bot takes damage, he will use the InstigatorActor that we have available and se our Target Actor to that instigator. 
		//Make sure that firstly we don't hit ourselves. 
		if(InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}

		// When we just died
		if(NewHealth <= 0.0f)
		{
			// Stop the behavior tree (Get the BT from controller)
			AAIController* AIC = Cast<AAIController>(GetController());

			// Make sure that is not a null pointer (maybe some other code earlier in the chain will already remove that AI controller from us)
			if(AIC)
			{
				//GetBrainComponent the base class of BT
				AIC->GetBrainComponent()->StopLogic("Killed");
			}


			// GOAL : RAGDOLL - See the corpse falling on the ground
			// We use GetMesh because they marked it as private
			// Set all the of the "bones" to simulate physics.
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			// Use the collision profile named Ragdoll which has collission enabled for query and physics!
			GetMesh()->SetCollisionProfileName("Ragdoll");

			// Set lifespan (how long until we call destroy Actor on ourselves)
			SetLifeSpan(10.0f);
		}
	}
//
//	// If the health of the character is below 0 and the delta was a "Damage Hit" then it means that he is dead and we disable the Player Controls.  
	if (NewHealth <= 0.0f && Delta < 0.0f)
	{
		//Destroy();
		//APlayerController* PC = Cast<APlayerController>(GetController());
		//DisableInput(PC);
	}
}




