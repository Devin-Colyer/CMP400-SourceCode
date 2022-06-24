// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CharacterMenuWidget.generated.h"

UCLASS()
class CMP400_2DSOULSLIKE_API UCharacterMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite)
		class UHorizontalBox* TitleBar;
	UPROPERTY(BlueprintReadWrite)
		class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(BlueprintReadOnly)
		int32 PageIndex;

public:
	UCharacterMenuWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual FReply NativeOnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;


	UFUNCTION(BlueprintCallable, Category = UISetup)
		void SetMenuFocus();
	UFUNCTION(BlueprintCallable, Category = UIControls)
		void NextPage();
	UFUNCTION(BlueprintCallable, Category = UIControls)
		void PrevPage();
	UFUNCTION(BlueprintCallable, Category = UIControls)
		void UpdateTitleBar();

public:
	UFUNCTION(BlueprintCallable, Category = UIControls)
		void SetPageIndex(int32 Index);
	
};
