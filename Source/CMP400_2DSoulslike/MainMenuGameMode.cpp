// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGameMode.h"

// AkAudio - Wwise
//#include "AkGameplayStatics.h"
//#include "AkAudioEvent.h"

// kismet
#include "Kismet/GameplayStatics.h"

// UI
#include "Blueprint/UserWidget.h"

// My headers
#include "MainMenuController.h"

AMainMenuGameMode::AMainMenuGameMode()
	: Super()
{
	//AudioDevice = FAkAudioDevice::Get();

	//static ConstructorHelpers::FObjectFinder<UAkAudioEvent>CalmMusicStateAsset(TEXT("AkAudioEvent'/Game/WwiseAudio/Events/Default_Work_Unit/Play_BGM_MainTheme.Play_BGM_MainTheme'"));
	//PlayMusicEvent = CalmMusicStateAsset.Object;
	//static ConstructorHelpers::FObjectFinder<UAkAudioEvent>StressMusicStateAsset(TEXT("AkAudioEvent'/Game/WwiseAudio/Events/Default_Work_Unit/Stop_BGM_MainTheme.Stop_BGM_MainTheme'"));
	//StopMusicEvent = StressMusicStateAsset.Object;

	static ConstructorHelpers::FClassFinder<UUserWidget>MenuAsset(TEXT("WidgetBlueprint'/Game/UI/Menus/MainMenuWidget.MainMenuWidget_C'"));
	WidgetClass = MenuAsset.Class;
	PlayerControllerClass = AMainMenuController::StaticClass();
}

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	//AudioDevice->PostEvent(PlayMusicEvent, UGameplayStatics::GetPlayerPawn(this, 0));

	if (WidgetClass) {
		MenuWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);

		// if instantiation successful, add to viewport
		if (MenuWidget)
			MenuWidget->AddToViewport();
	}
}

void AMainMenuGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	//AudioDevice->PostEvent(StopMusicEvent, UGameplayStatics::GetPlayerPawn(this, 0));
}

