// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SaveGame.h"
#include "OptionsSaveGame.generated.h"

UCLASS()
class CMP400_2DSOULSLIKE_API UOptionsSaveGame : public USaveGame
{
	GENERATED_BODY()

protected:
	// Audio
	UPROPERTY(VisibleAnywhere, Category = Audio)
		float MasterVolume;
	UPROPERTY(VisibleAnywhere, Category = Audio)
		float GameVolume;
	UPROPERTY(VisibleAnywhere, Category = Audio)
		float MusicVolume;
	UPROPERTY(VisibleAnywhere, Category = Audio)
		float AmbientVolume;
	UPROPERTY(VisibleAnywhere, Category = Display)
		bool bEnableSoundCompass;
	UPROPERTY(VisibleAnywhere, Category = Display)
		bool bEnableHighVisMode;
	UPROPERTY(VisibleAnywhere, Category = Display)
		TEnumAsByte<EWindowMode::Type> WindowMode;

public:
	UOptionsSaveGame();

protected:

public:
	UFUNCTION(BlueprintCallable)
		void SetAudioOptions(bool EnableSoundCompass, bool EnableHighVisMode, float InMasterVolume, float InGameVolume, float InMusicVolume, float InAmbientVolume, TEnumAsByte<EWindowMode::Type> InWindowMode);

	UFUNCTION(BlueprintPure)
		FORCEINLINE float GetMasterVolume() { return MasterVolume; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE float GetGameVolume() { return GameVolume; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE float GetMusicVolume() { return MusicVolume; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE float GetAmbientVolume() { return AmbientVolume; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool GetEnableSoundCompass() { return bEnableSoundCompass; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool GetEnableHighVisMode() { return bEnableHighVisMode; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE TEnumAsByte<EWindowMode::Type> GetWindowMode() { return WindowMode; }
	
};
