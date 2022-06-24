// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "GamepadUseableButton.generated.h"

UCLASS()
class CMP400_2DSOULSLIKE_API UGamepadUseableButton : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite)
		class UButton* Button;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FText PageText;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bIsActive;

public:
	UGamepadUseableButton(const FObjectInitializer& ObjectInitializer);

protected:
	UFUNCTION(BlueprintPure)
		FSlateColor SetActiveColor();

	UFUNCTION(BlueprintCallable)
		void SetAsHovered();

public:
	UFUNCTION(BlueprintCallable)
		void SetHovered(bool Active);

};
