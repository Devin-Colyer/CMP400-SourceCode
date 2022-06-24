// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "StatsWidget.generated.h"

UCLASS()
class CMP400_2DSOULSLIKE_API UStatsWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite)
		UWidget* DefaultFocusWidget;
	UPROPERTY(BlueprintReadWrite)
		TArray<class UButton*> ButtonArr;
	UPROPERTY(BlueprintReadWrite)
		FButtonStyle ItemStyle;
	UPROPERTY(BlueprintReadWrite)
		TArray<class UButton*> HotbarButtonArr;
	UPROPERTY(BlueprintReadWrite)
		TArray<class UImage*> HotbarImageArr;

	class UTexture2D* ItemBackplate;
	class UTexture2D* SelectedItemBackplate;

	UPROPERTY(BlueprintReadOnly)
		int32 HealthStat;
	UPROPERTY(BlueprintReadOnly)
		int32 StaminaStat;
	UPROPERTY(BlueprintReadOnly)
		int32 AttackPowerStat;
	UPROPERTY(BlueprintReadOnly)
		int32 ArcaneStat;

	UPROPERTY(BlueprintReadOnly)
		int32 HpStaPoints;
	UPROPERTY(BlueprintReadOnly)
		int32 AtkArcPoints;

public:
	UStatsWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	//virtual FReply NativeOnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;

	UFUNCTION(BlueprintCallable, Category = StatButtons)
		void UpgradeHealth();
	UFUNCTION(BlueprintCallable, Category = StatButtons)
		void UpgradeStamina();
	UFUNCTION(BlueprintCallable, Category = StatButtons)
		void UpgradeAttackPower();
	UFUNCTION(BlueprintCallable, Category = StatButtons)
		void UpgradeArcane();

	UFUNCTION(BlueprintCallable, Category = StatButtons)
		void HpStaButton();
	UFUNCTION(BlueprintCallable, Category = StatButtons)
		void AtkArcButton();

public:

};
