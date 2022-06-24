// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulslikePlayerController.h"

#include "Kismet/GameplayStatics.h"

#include "Blueprint/UserWidget.h"

#include "CharacterMenuWidget.h"
#include "PlayerHUD.h"

ASoulslikePlayerController::ASoulslikePlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget>PauseAsset(TEXT("WidgetBlueprint'/Game/UI/Menus/CharacterMenuWidget.CharacterMenuWidget_C'"));
	PauseWidgetClass = PauseAsset.Class;

	bMenuOpen = false;
	bUsingGamepad = false;
}

void ASoulslikePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Pause", IE_Pressed, this, &ASoulslikePlayerController::PauseButton);
}

bool ASoulslikePlayerController::InputKey(FKey Key, EInputEvent EventType, float AmountDepressed, bool bGamepad)
{
	Super::InputKey(Key, EventType, AmountDepressed, bGamepad);
	
	bUsingGamepad = bGamepad;

	return false;
}

void ASoulslikePlayerController::PauseButton()
{
	if (!bMenuOpen) {
		PauseGame();
	}
	else {
		UnpauseGame();
	}
}

void ASoulslikePlayerController::PauseGame()
{
	if (PauseWidgetClass && !PauseWidget)
		PauseWidget = CreateWidget<UUserWidget>(GetWorld(), PauseWidgetClass);

	// If instantiation successful, add to viewport
	if (PauseWidget && !PauseWidget->IsInViewport()) {
		PauseWidget->AddToViewport();
		//PauseWidget->SetKeyboardFocus();
	}

	bShowMouseCursor = true;
	bMenuOpen = true;

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD<APlayerHUD>()->ReorderDecal();
}

void ASoulslikePlayerController::UnpauseGame()
{
	if (PauseWidget) {
		PauseWidget->RemoveFromParent();
		PauseWidget = nullptr;
	}

	bShowMouseCursor = false;
	bMenuOpen = false;
}

void ASoulslikePlayerController::SetMenuIndex(int32 Index)
{
	Cast<UCharacterMenuWidget>(PauseWidget)->SetPageIndex(Index);
}
