// Fill out your copyright notice in the Description page of Project Settings.


#include "Hazard_Collision.h"

// Components
#include "Components/BoxComponent.h"

// Helpers
#include "Kismet/GameplayStatics.h"

// Sets default values
AHazard_Collision::AHazard_Collision()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Hitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hitbox"));
	Hitbox->SetGenerateOverlapEvents(true);
	Hitbox->SetCollisionResponseToChannels(ECollisionResponse::ECR_Overlap);

	Hitbox->OnComponentBeginOverlap.AddDynamic(this, &AHazard_Collision::OnComponentOverlap);

	HazardDamage = 10.f;

}

void AHazard_Collision::OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
		UGameplayStatics::ApplyDamage(OtherActor, HazardDamage, nullptr, this, UDamageType::StaticClass());
}

