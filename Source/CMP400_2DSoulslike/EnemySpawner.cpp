// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemySpawner.h"

#include "CharacterBase.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	Spawn();
}

void AEnemySpawner::Spawn()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor(CharacterToSpawn.Get(), &GetActorTransform(), SpawnParams);
}