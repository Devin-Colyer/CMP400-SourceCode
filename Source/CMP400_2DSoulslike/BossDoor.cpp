// Fill out your copyright notice in the Description page of Project Settings.

#include "BossDoor.h"

#include "PaperTileMapComponent.h"
#include "PaperTileMap.h"
#include "Components/BoxComponent.h"

#include "Enemy_BossSummoner.h"

// Sets default values
ABossDoor::ABossDoor()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UPaperTileMap>DefaultMapAsset(TEXT("PaperTileMap'/Game/Tilesets/DefaultTileMap.DefaultTileMap'"));
	DeafultDoorMap = DefaultMapAsset.Object;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	DoorMapComp = CreateDefaultSubobject<UPaperTileMapComponent>(TEXT("Map"));
	DoorMapComp->SetupAttachment(RootComponent);

	BossVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Volume"));
	BossVolume->SetupAttachment(RootComponent);
	BossVolume->SetBoxExtent(FVector(32.f, 32.f, 32.f));
	BossVolume->SetGenerateOverlapEvents(true);

	bBossDetected = true;
}

// Called every frame
void ABossDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bBossDetected || !DoorMap) {
		DoorMapComp->SetTileMap(DeafultDoorMap);
	}
	else {
		DoorMapComp->SetTileMap(DoorMap);
	}

	bBossDetected = false;
	TArray<AActor*>OverlappingActors;
	BossVolume->GetOverlappingActors(OverlappingActors, AEnemy_BossSummoner::StaticClass());
	for (AActor* Actor : OverlappingActors) {
		if (Cast<AEnemy_BossSummoner>(Actor)) {
			bBossDetected = true;
			break;
		}
	}
}

