// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableWidget.h"

#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "SoulslikePlayerController.h"

UInteractableWidget::UInteractableWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PromptTemplate = FText::FromString("Press {Key} to {Action}");
}

FText UInteractableWidget::GetKey(FName InActionName)
{
	UInputSettings* InputSettings = UInputSettings::GetInputSettings();

	TArray<FInputActionKeyMapping>Mappings;
	InputSettings->GetActionMappingByName(InActionName, Mappings);
	bool bUsingGamepad = Cast<ASoulslikePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->IsUsingGamepad();

	for (FInputActionKeyMapping Mapping : Mappings) {
		bool bFoundCorrectKey = (Mapping.Key.IsGamepadKey() && bUsingGamepad) || (!Mapping.Key.IsGamepadKey() && !bUsingGamepad);
		if (bFoundCorrectKey)
			return Mapping.Key.GetDisplayName();
	}
	return FText::FromString("Null");
}

FText UInteractableWidget::GetAxis(FName InAxisName, int32 Scale)
{
	UInputSettings* InputSettings = UInputSettings::GetInputSettings();

	TArray<FInputAxisKeyMapping>Mappings;
	InputSettings->GetAxisMappingByName(InAxisName, Mappings);
	bool bUsingGamepad = Cast<ASoulslikePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->IsUsingGamepad();

	for (FInputAxisKeyMapping Mapping : Mappings) {
		bool bFoundCorrectKey = (Mapping.Key.IsGamepadKey() && bUsingGamepad) || (!Mapping.Key.IsGamepadKey() && !bUsingGamepad);
		if (bFoundCorrectKey && Mapping.Scale == Scale)
			return Mapping.Key.GetDisplayName();
	}
	return FText::FromString("Null");
}

FText UInteractableWidget::GetPromptText()
{
	FFormatOrderedArguments Args;
	for (size_t i = 0; i < PromptArgs.Num(); i++)
	{
		Args.Add(FFormatArgumentValue(PromptArgs[i]));
	}
	return FText::Format(PromptTemplate, Args);
}
