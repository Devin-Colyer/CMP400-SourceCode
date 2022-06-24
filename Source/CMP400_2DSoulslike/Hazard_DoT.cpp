// Fill out your copyright notice in the Description page of Project Settings.

#include "Hazard_DoT.h"

// Components
#include "Components/BoxComponent.h"

// Helpers
#include "Kismet/GameplayStatics.h"

// My Headers
#include "CharacterBase.h"

// Sets default values
AHazard_DoT::AHazard_DoT()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Hitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hitbox"));
	Hitbox->SetGenerateOverlapEvents(true);
	Hitbox->SetCollisionResponseToChannels(ECollisionResponse::ECR_Overlap);

	Hitbox->OnComponentEndOverlap.AddDynamic(this, &AHazard_DoT::OnEndOverlap);

	HazardDamage = 10.f;
	TickFrequency = 0.5f;
}

// Called every frame
void AHazard_DoT::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<AActor*> OverlappingActors;
	Hitbox->GetOverlappingActors(OverlappingActors, ACharacterBase::StaticClass());
	for (AActor* Actor : OverlappingActors) {
		if (Actor != this && !DamageTimer.IsValid()) {
			if (!TargetCharacters.Contains(Actor))
				TargetCharacters.Add(Actor);

			if (!DamageTimer.IsValid())
				GetWorldTimerManager().SetTimer(DamageTimer, this, &AHazard_DoT::ApplyDamage, TickFrequency, true, 0.f);
		}
	}

	if (TargetCharacters.Num() <= 0)
		GetWorldTimerManager().ClearTimer(DamageTimer);
}

void AHazard_DoT::ApplyDamage()
{
	for (AActor* Actor : TargetCharacters)
		if (Actor)
			UGameplayStatics::ApplyDamage(Actor, HazardDamage, nullptr, this, UDamageType::StaticClass());

	if (TargetCharacters.Num() <= 0)
		GetWorldTimerManager().ClearTimer(DamageTimer);
}

void AHazard_DoT::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (TargetCharacters.Contains(OtherActor))
		TargetCharacters.Remove(OtherActor);

	if (TargetCharacters.Num() <= 0)
		GetWorldTimerManager().ClearTimer(DamageTimer);
}
