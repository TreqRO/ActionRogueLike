// Fill out your copyright notice in the Description page of Project Settings.


#include "SSharedProjectileClass.h"

// Sets default values
ASSharedProjectileClass::ASSharedProjectileClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASSharedProjectileClass::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASSharedProjectileClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

