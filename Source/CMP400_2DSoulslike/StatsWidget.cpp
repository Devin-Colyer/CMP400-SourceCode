// Fill out your copyright notice in the Description page of Project Settings.


#include "StatsWidget.h"

#include "Components/Button.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

UStatsWidget::UStatsWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UTexture2D>ItemAsset(TEXT("Texture2D'/Game/Textures/Sweenus_HUD/ItemsBackplate.ItemsBackplate'"));
	ItemBackplate = ItemAsset.Object;
	static ConstructorHelpers::FObjectFinder<UTexture2D>HoveredItemAsset(TEXT("Texture2D'/Game/Textures/Sweenus_HUD/SelectedItemsBackplate.SelectedItemsBackplate'"));
	SelectedItemBackplate = HoveredItemAsset.Object;

	HealthStat = 1;
	StaminaStat = 1;
	AttackPowerStat = 1;
	ArcaneStat = 1;

	HpStaPoints = 1;
	AtkArcPoints = 1;
}

void UStatsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ItemStyle.SetHovered(UWidgetBlueprintLibrary::MakeBrushFromTexture(SelectedItemBackplate, 61.f, 61.f));
	ItemStyle.SetPressed(UWidgetBlueprintLibrary::MakeBrushFromTexture(ItemBackplate, 61.f, 61.f));
}

void UStatsWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (HasKeyboardFocus())
		if (DefaultFocusWidget)
			DefaultFocusWidget->SetKeyboardFocus();

	for (UButton* Button : ButtonArr) {
		if (HotbarButtonArr.Contains(Button)) {
			if (Button->HasKeyboardFocus()) {
				ItemStyle.SetNormal(UWidgetBlueprintLibrary::MakeBrushFromTexture(SelectedItemBackplate, 61.f, 61.f));
			}
			else {
				ItemStyle.SetNormal(UWidgetBlueprintLibrary::MakeBrushFromTexture(ItemBackplate, 61.f, 61.f));
			}

			Button->SetStyle(ItemStyle);
		}
		else {
			if (Button->HasKeyboardFocus()) {
				Button->SetBackgroundColor(FLinearColor(FColor::FromHex("876E0EFF")));
			}
			else {
				Button->SetBackgroundColor(FLinearColor(FColor::FromHex("FFFFFF00")));
			}
		}
	}
}

void UStatsWidget::UpgradeHealth()
{
	HealthStat++;
}

void UStatsWidget::UpgradeStamina()
{
	StaminaStat++;
}

void UStatsWidget::UpgradeAttackPower()
{
	AttackPowerStat++;
}

void UStatsWidget::UpgradeArcane()
{
	ArcaneStat++;
}

void UStatsWidget::HpStaButton()
{
	if (HpStaPoints > 0) {
		UpgradeHealth();
		UpgradeStamina();
		HpStaPoints--;
	}
}

void UStatsWidget::AtkArcButton()
{
	if (AtkArcPoints > 0) {
		UpgradeAttackPower();
		UpgradeArcane();
		AtkArcPoints--;
	}
}
