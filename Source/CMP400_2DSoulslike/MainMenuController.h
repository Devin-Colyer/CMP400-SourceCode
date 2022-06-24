// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "MainMenuController.generated.h"

/**
 * 
 */
UCLASS()
class CMP400_2DSOULSLIKE_API AMainMenuController : public APlayerController
{
	GENERATED_BODY()

public:
	AMainMenuController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
protected:
	void BeginPlay() override;
};
