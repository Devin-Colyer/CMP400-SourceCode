// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Hazard_DoT.generated.h"

UCLASS()
class CMP400_2DSOULSLIKE_API AHazard_DoT : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Hazard)
		float HazardDamage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Hazard)
		float TickFrequency;

	struct FTimerHandle DamageTimer;

	class UBoxComponent* Hitbox;
	TArray<AActor*> TargetCharacters;
	
public:	
	// Sets default values for this actor's properties
	AHazard_DoT();

protected:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void ApplyDamage();

	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	

};
