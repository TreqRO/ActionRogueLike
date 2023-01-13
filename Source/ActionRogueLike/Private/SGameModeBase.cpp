// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include "EngineUtils.h"
#include "SAttributeComponent.h"
#include "AI/SAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"

// Start by setting a timer and the timer will just run every 2 seconds and run the EQS Query(The Spawn Bot) that we just created
// and once the query is done spawn a bot at the best result the query can give us


ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 2.0f;
}

void ASGameModeBase::StartPlay()
{
	// If not called none of our actors would receive the begin play more
	Super::StartPlay();

	// Continuous timer to spawn in more bots
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}


void ASGameModeBase::SpawnBotTimerElapsed()
{
	// Gives us the event trigger once we're finally done (it runs multiple frames).
	UEnvQueryInstanceBlueprintWrapper* QueryInstance =  UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if(ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryCompleted);
	}
	
}


void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	// Called after a few frames after we have triggered our EQS query and then this will actually give us the results(the location) where we want to spawn our bot.   

	if (QueryStatus !=  EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query failed!"));
		return;
	}

	// Every time we hit this query we can find what is the number of alive bots at this point in time
	int32 NrOfAliveBots = 0;
	// Better version of "Get Actor by class" by letting us specify which class we want to grab.
	// Let us get a pointer to all of the instances of our characters in the world
	for(TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(Bot->GetComponentByClass(USAttributeComponent::StaticClass()));
		if(AttributeComp && AttributeComp->IsAlive())
		{
			NrOfAliveBots++;
		}
	}
	float MaxBotCount = 10.0f;

	if (DifficultyCurve)
	{
		//In case we have a Difficulty Curve, the maximum number of Bots spawned is depended by the value of the curve at time X.

		//The time is just the X Axis
		// At time 0 -> Value(Spawn 1 Enemy)
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}


	if(NrOfAliveBots >= MaxBotCount)
	{
		// we exit the function (hence not spawn anymore)
		return;
	}


	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	
	if(Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);

	}
}

