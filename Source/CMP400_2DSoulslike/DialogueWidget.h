// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InteractableWidget.h"
#include "DialogueWidget.generated.h"

UCLASS()
class CMP400_2DSOULSLIKE_API UDialogueWidget : public UInteractableWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		TArray<FText>DialogueLines;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		int32 CurrentLine;

public:
	UDialogueWidget(const FObjectInitializer& ObjectInitializer);

protected:
	UFUNCTION(BlueprintCallable, Category = UI)
		FORCEINLINE FText GetDialogueLine() { return DialogueLines[CurrentLine]; }

public:
	UFUNCTION(BlueprintCallable, Category = UI)
		void NextLine();
};
