// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InteractableBase.h"
#include "Interactable_NextLevel.generated.h"

UCLASS()
class CMP400_2DSOULSLIKE_API AInteractable_NextLevel : public AInteractableBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInteractable_NextLevel();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSoftObjectPtr<UWorld> Level;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName Options;

	float TimeToComplete;

protected:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void NextLevel();

public:
	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
};
