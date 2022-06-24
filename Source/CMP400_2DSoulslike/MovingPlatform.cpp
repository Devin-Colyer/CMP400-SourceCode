// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

#include "PaperTileMapComponent.h"
#include "Components/BoxComponent.h"

AMovingPlatform::AMovingPlatform()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Map->Mobility = EComponentMobility::Movable;
	
	TriggerBox->SetRelativeLocation(FVector(96.f, 0.f, 110.f));
	
	StickyBox = CreateDefaultSubobject<UBoxComponent>(TEXT("StickyBox"));
	StickyBox->SetupAttachment(Map);
	StickyBox->SetBoxExtent(FVector(32.f, 32.f, 16.f));
	StickyBox->SetGenerateOverlapEvents(true);
	StickyBox->SetRelativeLocation(FVector(0.f, 0.f, -15.f));

	// Defaults
	PlatformSpeed = PlatformProgress = 0.f;
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	SetActorLocation(StartingLocation);
}

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float Time = FVector::Dist(StartingLocation, TargetLocation) / PlatformSpeed;

	PlatformProgress += (DeltaTime / Time);
	FVector OldLoc = GetActorLocation();
	FVector NewLoc = FMath::Lerp(StartingLocation, TargetLocation, PlatformProgress);
	SetActorLocation(NewLoc);

	if (PlatformProgress >= 1.f) {
		FVector Temp = TargetLocation;
		TargetLocation = StartingLocation;
		StartingLocation = Temp;
		PlatformProgress = 0.f;
	}
}
