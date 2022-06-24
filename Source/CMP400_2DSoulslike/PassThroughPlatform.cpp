// Fill out your copyright notice in the Description page of Project Settings.


#include "PassThroughPlatform.h"

#include "PaperTileMapComponent.h"
#include "Components/BoxComponent.h"

// My headers
#include "PlayerCharacter.h"

// Sets default values
APassThroughPlatform::APassThroughPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Map = CreateDefaultSubobject<UPaperTileMapComponent>(TEXT("Map"));
	Map->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	Map->SetupAttachment(RootComponent);

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	TriggerBox->SetupAttachment(Map);
	TriggerBox->SetBoxExtent(FVector(32.f, 32.f, 16.f));
	TriggerBox->SetGenerateOverlapEvents(true);
	TriggerBox->SetRelativeLocation(FVector(0.f, 0.f, 180.f));

	//TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &APassThroughPlatform::OnBeginOverlap);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &APassThroughPlatform::OnEndOverlap);
}

void APassThroughPlatform::Tick(float DeltaTime)
{
	TArray<AActor*>OverlappingActors;
	TriggerBox->GetOverlappingActors(OverlappingActors, APlayerCharacter::StaticClass());
	for (AActor* Actor : OverlappingActors) {
		if (Cast<APlayerCharacter>(Actor))
			Map->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	}
}

void APassThroughPlatform::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
		if (Cast<APlayerCharacter>(OtherActor))
			Map->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
}