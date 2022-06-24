// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "MainMenuGameMode.generated.h"

UCLASS()
class CMP400_2DSOULSLIKE_API AMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMainMenuGameMode();

protected:
	UPROPERTY(EditAnywhere, Category = MainMenu)
		TSubclassOf<class UUserWidget> WidgetClass;
	UPROPERTY(EditAnywhere, Category = MainMenu)
		class UUserWidget* MenuWidget;

	// wwise
	//class FAkAudioDevice* AudioDevice;
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Music)
	//	class UAkAudioEvent* PlayMusicEvent;
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Music)
	//	class UAkAudioEvent* StopMusicEvent;

private:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
};
