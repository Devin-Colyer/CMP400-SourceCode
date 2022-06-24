// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "AttackHitbox.generated.h"

UCLASS()
class CMP400_2DSOULSLIKE_API AAttackHitbox : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UShapeComponent* Hitbox;
	AActor* OwnerActor;

public:
	// Sets default values for this actor's properties
	AAttackHitbox();

	// damage to deal
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 DamageValue;

	// damage type
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TSubclassOf<UDamageType> DamageType;

protected:
	void InitHitbox_Box(FVector BoxExtent);
	void InitHitbox_Sphere(float Radius);
	void InitHitbox_Capsule(float Radius, float HalfHeight);
	void InitComplete();

public:
	// Overloaded functions to support various hitbox shapes
	void SetupHitbox(FVector BoxExtent, AActor* ActorToIgnore, int32 InDamage = -1);
	void SetupHitbox(float Radius, AActor* ActorToIgnore, int32 InDamage = -1);
	void SetupHitbox(float Radius, float HalfHeight, AActor* ActorToIgnore, int32 InDamage = -1);

	// Overloaded functions to support various hitbox shapes
	void ChangeShape(FVector BoxExtent);
	void ChangeShape(float Radius);
	void ChangeShape(float Radius, float HalfHeight);

	void DamageEnemies();

};
