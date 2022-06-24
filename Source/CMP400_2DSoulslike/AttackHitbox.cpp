// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackHitbox.h"

#include "Kismet/GameplayStatics.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"

#include "CharacterBase.h"

// Sets default values
AAttackHitbox::AAttackHitbox()
{
	Hitbox = CreateDefaultSubobject<UShapeComponent>(TEXT("Root"));
	DamageValue = 20;
	OwnerActor = nullptr;
}

void AAttackHitbox::InitHitbox_Box(FVector BoxExtent)
{
	if (Hitbox)
		Hitbox->DestroyComponent();

	UBoxComponent* NewHitbox = NewObject<UBoxComponent>(this, UBoxComponent::StaticClass(), TEXT("Hitbox"));
	NewHitbox->InitBoxExtent(BoxExtent);
	Hitbox = NewHitbox;
}

void AAttackHitbox::InitHitbox_Sphere(float Radius)
{
	if (Hitbox)
		Hitbox->DestroyComponent();

	USphereComponent* NewHitbox = NewObject<USphereComponent>(this, USphereComponent::StaticClass(), TEXT("Hitbox"));
	NewHitbox->InitSphereRadius(Radius);
	Hitbox = NewHitbox;
}

void AAttackHitbox::InitHitbox_Capsule(float Radius, float HalfHeight)
{
	if (Hitbox)
		Hitbox->DestroyComponent();

	UCapsuleComponent* NewHitbox = NewObject<UCapsuleComponent>(this, UCapsuleComponent::StaticClass(), TEXT("Hitbox"));
	NewHitbox->InitCapsuleSize(Radius, HalfHeight);
	Hitbox = NewHitbox;
}

void AAttackHitbox::InitComplete()
{
	Hitbox->SetGenerateOverlapEvents(true);
	Hitbox->SetCollisionResponseToChannels(ECollisionResponse::ECR_Overlap);

	Hitbox->RegisterComponent();
	RootComponent = Hitbox;
}

void AAttackHitbox::SetupHitbox(FVector BoxExtent, AActor* ActorToIgnore, int32 InDamage)
{
	if (InDamage > 0)
		DamageValue = InDamage;

	OwnerActor = ActorToIgnore;

	InitHitbox_Box(BoxExtent);
	InitComplete();
}

void AAttackHitbox::SetupHitbox(float Radius, AActor* ActorToIgnore, int32 InDamage)
{
	if (InDamage > 0)
		DamageValue = InDamage;

	OwnerActor = ActorToIgnore;

	InitHitbox_Sphere(Radius);
	InitComplete();
}

void AAttackHitbox::SetupHitbox(float Radius, float HalfHeight, AActor* ActorToIgnore, int32 InDamage)
{
	if (InDamage > 0)
		DamageValue = InDamage;

	OwnerActor = ActorToIgnore;

	InitHitbox_Capsule(Radius, HalfHeight);
	InitComplete();
}

void AAttackHitbox::ChangeShape(FVector BoxExtent)
{
	UBoxComponent* Box = Cast<UBoxComponent>(Hitbox);
	if (Box) {
		Box->SetBoxExtent(BoxExtent);
	}
}

void AAttackHitbox::ChangeShape(float Radius)
{
	USphereComponent* Box = Cast<USphereComponent>(Hitbox);
	if (Box) {
		Box->SetSphereRadius(Radius);
	}
}

void AAttackHitbox::ChangeShape(float Radius, float HalfHeight)
{
	UCapsuleComponent* Box = Cast<UCapsuleComponent>(Hitbox);
	if (Box) {
		Box->SetCapsuleSize(Radius, HalfHeight);
	}
}

void AAttackHitbox::DamageEnemies()
{
	TArray<AActor*> OverlappingActors;
	TArray<UPrimitiveComponent*> OverlappingComps;
	Hitbox->GetOverlappingComponents(OverlappingComps);
	Hitbox->GetOverlappingActors(OverlappingActors, ACharacterBase::StaticClass());
	for (AActor* Actor : OverlappingActors) {
		if (Actor != OwnerActor)
			for (UActorComponent* Comp : Actor->GetComponentsByClass(UCapsuleComponent::StaticClass())) {
				if (OverlappingComps.Contains(Comp)) {
					UGameplayStatics::ApplyDamage(Actor, DamageValue, nullptr, this, DamageType);
					break;
				}
			}
	}
}
