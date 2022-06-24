// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

AMainMenuController::AMainMenuController(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
}

void AMainMenuController::BeginPlay()
{
	UWidgetBlueprintLibrary::SetInputMode_UIOnly(this);
	bShowMouseCursor = true;

	bAutoManageActiveCameraTarget = false;
	InputYawScale = 0.f;
	InputPitchScale = 0.f;
	InputRollScale = 0.f;
}
