// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InteractableBase.h"
#include "Interactable_Dialogue.generated.h"

UCLASS()
class CMP400_2DSOULSLIKE_API AInteractable_Dialogue : public AInteractableBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = UI)
		TSubclassOf<class UDialogueWidget> DialogueWidgetClass;
	UPROPERTY(EditAnywhere, Category = UI)
		class UDialogueWidget* DialogueWidget;

public:
	// Sets default values for this actor's properties
	AInteractable_Dialogue();

protected:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void NextLine();

public:
	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
};
