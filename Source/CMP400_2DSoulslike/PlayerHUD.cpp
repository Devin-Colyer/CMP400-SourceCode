// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"

#include "Blueprint/UserWidget.h"

// Helpers
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

// My headers
#include "AudioCompassWidget.h"
#include "OptionsSaveGame.h"

APlayerHUD::APlayerHUD()
{
	// Set this game mode to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//get then set HUD widget class
	static ConstructorHelpers::FClassFinder<UUserWidget>HUDAsset(TEXT("WidgetBlueprint'/Game/UI/HUD/HUDWidget.HUDWidget_C'"));
	HUDWidgetClass = HUDAsset.Class;
	static ConstructorHelpers::FClassFinder<UUserWidget>CompassWidgetAsset(TEXT("WidgetBlueprint'/Game/UI/HUD/AudioCompassWidget.AudioCompassWidget_C'"));
	CompassWidgetClass = CompassWidgetAsset.Class;
	static ConstructorHelpers::FClassFinder<UUserWidget>MacularDegenerationAsset(TEXT("WidgetBlueprint'/Game/UI/MacularDegeneration.MacularDegeneration_C'"));
	MacularDegenerationClass = MacularDegenerationAsset.Class;
}

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	// if the HUD widget class exists, create instance of it
	if (HUDWidgetClass) {
		HUDWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);

		// if instantiation successful, add to viewport
		if (HUDWidget)
			HUDWidget->AddToViewport();
	}

	if (CompassWidgetClass) {
		CompassWidget = CreateWidget<UAudioCompassWidget>(GetWorld(), CompassWidgetClass);

		// if instantiation successful, add to viewport
		if (CompassWidget)
			CompassWidget->AddToViewport();
	}

	if (MacularDegenerationClass) {
		MacularDegenerationWidget = CreateWidget<UUserWidget>(GetWorld(), MacularDegenerationClass);

		// if instantiation successful, add to viewport
		if (MacularDegenerationWidget)
			MacularDegenerationWidget->AddToViewport();
	}
}

void APlayerHUD::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UGameplayStatics::DoesSaveGameExist("SettingsSlot", 0) ?
		OptionsSaveGame = Cast<UOptionsSaveGame>(UGameplayStatics::LoadGameFromSlot("SettingsSlot", 0)) :
		OptionsSaveGame = Cast<UOptionsSaveGame>(UGameplayStatics::CreateSaveGameObject(UOptionsSaveGame::StaticClass()));

	if (OptionsSaveGame->GetEnableSoundCompass()) {
		CompassWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else {
		CompassWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (OptionsSaveGame->GetEnableHighVisMode()) {
		MacularDegenerationWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else {
		MacularDegenerationWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void APlayerHUD::SfxTriggered(FVector SfxLocation, float SfxDuration)
{
	CompassWidget->OnSfxTriggered.Broadcast(SfxLocation, SfxDuration);
}

void APlayerHUD::ReorderDecal()
{
	if (MacularDegenerationWidget) {
		MacularDegenerationWidget->RemoveFromParent();
		MacularDegenerationWidget = nullptr;
	}

	if (MacularDegenerationClass) {
		MacularDegenerationWidget = CreateWidget<UUserWidget>(GetWorld(), MacularDegenerationClass);

		// if instantiation successful, add to viewport
		if (MacularDegenerationWidget)
			MacularDegenerationWidget->AddToViewport();
	}
}



