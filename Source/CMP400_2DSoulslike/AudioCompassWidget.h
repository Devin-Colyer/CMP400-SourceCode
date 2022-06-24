// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include <mutex>
#include "AudioCompassWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSfxTriggered, FVector, SfxLocation, float, SfxDuration);

UCLASS()
class UCompassArrow : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
		class UWidget* Arrow;
	UPROPERTY(BlueprintReadWrite)
		FVector2D SfxSource;
};

UCLASS()
class CMP400_2DSOULSLIKE_API UAudioCompassWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	TSubclassOf<UWidget> ArrowWidgetClass;
	TArray<UCompassArrow*> CompassArrows;

	std::mutex CompassMutex;

	FTimerManager TimerManager;
	UTexture2D* ArrowTexture;

public:
	UAudioCompassWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintCallable)
		FOnSfxTriggered OnSfxTriggered;

protected:
	// Overridden methods
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
		void SfxTriggered(FVector SfxLocation, float SfxDuration);

	// Timer Function
	UFUNCTION()
		void RemoveArrow(UCompassArrow* Arrow, FTimerHandle AssociatedTimer);
};
