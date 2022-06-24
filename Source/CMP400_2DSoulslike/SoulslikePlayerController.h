// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "SoulslikePlayerController.generated.h"

UCLASS()
class CMP400_2DSOULSLIKE_API ASoulslikePlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	// Pause Widget
	UPROPERTY(EditAnywhere, Category = PauseMenu)
		TSubclassOf<class UUserWidget> PauseWidgetClass;
	UPROPERTY(EditAnywhere, Category = PauseMenu)
		class UUserWidget* PauseWidget;

	bool bMenuOpen;
	bool bUsingGamepad;

public:
	ASoulslikePlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void SetupInputComponent() override;
	virtual bool InputKey(FKey Key, EInputEvent EventType, float AmountDepressed, bool bGamepad) override;
	virtual void PauseButton();

	void PauseGame();
	UFUNCTION(BlueprintCallable, Category = PauseMenu)
		void UnpauseGame();

public:
	UFUNCTION(BlueprintPure)
		bool IsMenuOpen() { return bMenuOpen; }
	UFUNCTION(BlueprintPure)
		bool IsUsingGamepad() { return bUsingGamepad; }
	void SetMenuIndex(int32 Index);
};
