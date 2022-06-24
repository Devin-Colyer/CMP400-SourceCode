// Fill out your copyright notice in the Description page of Project Settings.


#include "OptionsSaveGame.h"

UOptionsSaveGame::UOptionsSaveGame()
{
    bEnableSoundCompass = false;
    bEnableHighVisMode = false;
    MasterVolume = 75.f;
    GameVolume = 75.f;
    MusicVolume = 75.f;
    AmbientVolume = 75.f;
    WindowMode = EWindowMode::Type::Fullscreen;
}

void UOptionsSaveGame::SetAudioOptions(bool EnableSoundCompass, bool EnableHighVisMode, float InMasterVolume, float InGameVolume, float InMusicVolume, float InAmbientVolume, TEnumAsByte<EWindowMode::Type> InWindowMode)
{
    bEnableSoundCompass = EnableSoundCompass;
    bEnableHighVisMode = EnableHighVisMode;
    MasterVolume = InMasterVolume;
    GameVolume = InGameVolume;
    MusicVolume = InMusicVolume;
    AmbientVolume = InAmbientVolume;
    WindowMode = InWindowMode;
}

