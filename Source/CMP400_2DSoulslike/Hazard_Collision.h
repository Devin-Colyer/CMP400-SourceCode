// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Hazard_Collision.generated.h"

UCLASS()
class CMP400_2DSOULSLIKE_API AHazard_Collision : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Hazard)
		float HazardDamage;
	class UBoxComponent* Hitbox;
	
public:	
	// Sets default values for this actor's properties
	AHazard_Collision();

protected:
	UFUNCTION()
		void OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:

};
