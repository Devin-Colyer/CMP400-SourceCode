// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "InteractableBase.generated.h"

// Event dispatchers
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteract);

UCLASS()
class CMP400_2DSOULSLIKE_API AInteractableBase : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USphereComponent* Range;

	UPROPERTY(EditAnywhere, Category = UI)
		TSubclassOf<class UInteractableWidget> PromptWidgetClass;
	UPROPERTY(EditAnywhere, Category = UI)
		class UInteractableWidget* PromptWidget;

	bool bInteracting;

public:
	// Sets default values for this actor's properties
	AInteractableBase();

	UPROPERTY(BlueprintAssignable)
		FOnInteract OnInteract;

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void AddPrompt();

public:
	virtual void RemovePrompt(AActor* OtherActor);

};
