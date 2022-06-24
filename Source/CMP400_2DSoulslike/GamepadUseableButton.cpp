// Fill out your copyright notice in the Description page of Project Settings.


#include "GamepadUseableButton.h"

#include "Components/PanelWidget.h"
#include "Kismet/GameplayStatics.h"

#include "SoulslikePlayerController.h"

UGamepadUseableButton::UGamepadUseableButton(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsFocusable = false;
}

FSlateColor UGamepadUseableButton::SetActiveColor()
{
	if (bIsActive) {
		return FSlateColor(FColor::FromHex("876E0EFF"));
	}
	else {
		return FSlateColor(FColor::FromHex("FFFFFFFF"));
	}
}

void UGamepadUseableButton::SetAsHovered()
{
	for (UWidget* Widget : GetParent()->GetAllChildren()) {
		UGamepadUseableButton* ButtonPtr = Cast<UGamepadUseableButton>(Widget);
		if (ButtonPtr)
			ButtonPtr->SetHovered(false);
	}
	int32 MyIndex;
	GetParent()->GetAllChildren().Find(this, MyIndex);
	Cast<ASoulslikePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->SetMenuIndex(MyIndex);
}

void UGamepadUseableButton::SetHovered(bool Active)
{
	bIsActive = Active;
}
