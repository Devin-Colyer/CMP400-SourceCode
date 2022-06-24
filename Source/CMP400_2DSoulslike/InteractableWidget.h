// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "InteractableWidget.generated.h"

UCLASS()
class CMP400_2DSOULSLIKE_API UInteractableWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UInteractableWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		FText PromptTemplate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		TArray<FText> PromptArgs;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		FText ActionText;

protected:
	UFUNCTION(BlueprintPure)
		FText GetKey(FName InActionName);
	UFUNCTION(BlueprintPure)
		FText GetAxis(FName InAxisName, int32 Scale);

	UFUNCTION(BlueprintPure)
		virtual FText GetPromptText();
};
