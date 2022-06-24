// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterMenuWidget.h"

#include "Components/HorizontalBox.h"
#include "Components/WidgetSwitcher.h"

#include "GamepadUseableButton.h"

UCharacterMenuWidget::UCharacterMenuWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsFocusable = true;
	PageIndex = 0;
}

void UCharacterMenuWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (MenuSwitcher)
		MenuSwitcher->SetActiveWidgetIndex(PageIndex);
}

FReply UCharacterMenuWidget::NativeOnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey().GetDisplayName().ToString().Equals(FString("Gamepad Right Shoulder"))) {
		NextPage();
	}
	else if (InKeyEvent.GetKey().GetDisplayName().ToString().Equals(FString("Gamepad Left Shoulder"))) {
		PrevPage();
	}
	else {
		return FReply::Unhandled();
	}

	return FReply::Handled();
}

void UCharacterMenuWidget::SetPageIndex(int32 Index)
{
	if (Index >= 0 && Index <= 2) {
		PageIndex = Index;
	}
	UpdateTitleBar();
}

void UCharacterMenuWidget::SetMenuFocus()
{
	MenuSwitcher->GetChildAt(PageIndex)->SetKeyboardFocus();
}

void UCharacterMenuWidget::NextPage()
{
	if (++PageIndex > 2) {
		PageIndex = 0;
	}
	UpdateTitleBar();
}

void UCharacterMenuWidget::PrevPage()
{
	if (--PageIndex < 0) {
		PageIndex = 2;
	}
	UpdateTitleBar();
}

void UCharacterMenuWidget::UpdateTitleBar()
{
	for (UWidget* Button : TitleBar->GetAllChildren())
		Cast<UGamepadUseableButton>(Button)->SetHovered(false);

	Cast<UGamepadUseableButton>(TitleBar->GetChildAt(PageIndex))->SetHovered(true);
	MenuSwitcher->GetChildAt(PageIndex)->SetKeyboardFocus();
}
