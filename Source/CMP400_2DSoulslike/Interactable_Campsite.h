// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InteractableBase.h"
#include "Interactable_Campsite.generated.h"

UCLASS()
class CMP400_2DSOULSLIKE_API AInteractable_Campsite : public AInteractableBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInteractable_Campsite();

protected:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void RestAtCamp();

public:
	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
