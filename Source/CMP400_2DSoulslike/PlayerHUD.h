// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

UCLASS()
class CMP400_2DSOULSLIKE_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

protected:
	// HUD widget blueprint class
	UPROPERTY(EditAnywhere, Category = HUDWidget)
		TSubclassOf<class UUserWidget> HUDWidgetClass;
	// HUD widget pointer
	UPROPERTY(EditAnywhere, Category = HUDWidget)
		class UUserWidget* HUDWidget;

	// Audio compass widget blueprint class
	UPROPERTY(EditAnywhere, Category = CompassWidget)
		TSubclassOf<class UAudioCompassWidget> CompassWidgetClass;
	// Compass widget pointer
	UPROPERTY(EditAnywhere, Category = CompassWidget)
		class UAudioCompassWidget* CompassWidget;

	// Macular degeneration widget blueprint class
	UPROPERTY(EditAnywhere, Category = MacularDegeneration)
		TSubclassOf<class UUserWidget> MacularDegenerationClass;
	// Macular degeneration widget pointer
	UPROPERTY(EditAnywhere, Category = MacularDegeneration)
		class UUserWidget* MacularDegenerationWidget;

	class UOptionsSaveGame* OptionsSaveGame;

public:
	APlayerHUD();

protected:
	// overridden methods
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable)
		void SfxTriggered(FVector SfxLocation, float SfxDuration);
	UFUNCTION(BlueprintCallable)
		void ReorderDecal();
	
};
