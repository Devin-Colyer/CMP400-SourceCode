// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "AdventureGameMode.generated.h"

UCLASS(minimalapi)
class AAdventureGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	// Pointer to player character
	class APlayerCharacter* PlayerCharacter;

	// wwise
	//class FAkAudioDevice* AudioDevice;
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Music)
	//	class UAkAudioEvent* PlayMusicEvent;
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Music)
	//	class UAkAudioEvent* StopMusicEvent;

public:
	AAdventureGameMode();

protected:
	// Overridden methods
	virtual void BeginPlay() override;
	//virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	
};
