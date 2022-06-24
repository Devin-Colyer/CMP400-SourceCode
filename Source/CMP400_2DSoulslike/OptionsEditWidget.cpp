// Fill out your copyright notice in the Description page of Project Settings.


#include "OptionsEditWidget.h"

// Kismet
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Paper 2d
#include "PaperTileSet.h"
#include "PaperTileMap.h"
#include "PaperTileMapComponent.h"
#include "PaperFlipbookComponent.h"

// UI
#include "Components/Slider.h"
#include "Components/CheckBox.h"
#include "Components/ComboBoxString.h"

// Framework
#include "GameFramework/InputSettings.h"
#include "GameFramework/GameUserSettings.h"

// AkAudio - Wwise
#include "AkGameplayStatics.h"
#include "AkRtpc.h"

// My headers
#include "OptionsSaveGame.h"
#include "MyPaperSpriteActor.h"
#include "CharacterBase.h"
#include "TrollSpellEffect.h"

UOptionsEditWidget::UOptionsEditWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//AudioDevice = FAkAudioDevice::Get();

	//static ConstructorHelpers::FObjectFinder<UAkRtpc>MasterVolumeParamAsset(TEXT("AkRtpc'/Game/WwiseAudio/Game_Parameters/Default_Work_Unit/MasterVolParam.MasterVolParam'"));
	//MasterVolumeParam = MasterVolumeParamAsset.Object;
	//static ConstructorHelpers::FObjectFinder<UAkRtpc>GameVolumeParamAsset(TEXT("AkRtpc'/Game/WwiseAudio/Game_Parameters/Default_Work_Unit/GameVolParam.GameVolParam'"));
	//GameVolumeParam = GameVolumeParamAsset.Object;
	//static ConstructorHelpers::FObjectFinder<UAkRtpc>MusicVolumeParamAsset(TEXT("AkRtpc'/Game/WwiseAudio/Game_Parameters/Default_Work_Unit/MusicVolParam.MusicVolParam'"));
	//MusicVolumeParam = MusicVolumeParamAsset.Object;

	static ConstructorHelpers::FObjectFinder<UPaperTileSet>LevelTileSetAsset(TEXT("PaperTileSet'/Game/Tilesets/cavesofgallet_TileSet.cavesofgallet_TileSet'"));
	LevelTileSet = LevelTileSetAsset.Object;

	static ConstructorHelpers::FObjectFinder<UTexture2D>BasicTileSetAsset(TEXT("Texture2D'/Game/Textures/AdamSaltsman_Caverns/cavesofgallet_tiles_transparrent.cavesofgallet_tiles_transparrent'"));
	BaseTileSetTexture = BasicTileSetAsset.Object;
	static ConstructorHelpers::FObjectFinder<UTexture2D>HighVisTileSetAsset(TEXT("Texture2D'/Game/Textures/AdamSaltsman_Caverns/cavesofgallet_tiles_HighVis.cavesofgallet_tiles_HighVis'"));
	HighVisTileSetTexture = HighVisTileSetAsset.Object;

	// Levels
	static ConstructorHelpers::FObjectFinder<UPaperTileMap>Tutorial_TileMapAsset(TEXT("PaperTileMap'/Game/Tilesets/Levels/Tutorial_TileMap.Tutorial_TileMap'"));
	TileMapsToRebuild.Add(Tutorial_TileMapAsset.Object);
	static ConstructorHelpers::FObjectFinder<UPaperTileMap>Platforming_TileMapAsset(TEXT("PaperTileMap'/Game/Tilesets/Levels/Platforming_TileMap.Platforming_TileMap'"));
	TileMapsToRebuild.Add(Platforming_TileMapAsset.Object);
	static ConstructorHelpers::FObjectFinder<UPaperTileMap>Dodge_TileMapAsset(TEXT("PaperTileMap'/Game/Tilesets/Levels/Dodge_TileMap.Dodge_TileMap'"));
	TileMapsToRebuild.Add(Dodge_TileMapAsset.Object);
	/*static ConstructorHelpers::FObjectFinder<UPaperTileMap>TutorialTileMapAsset(TEXT("PaperTileMap'/Game/Tilesets/Levels/Tutorial_TileMap.Tutorial_TileMap'"));
	TileMapsToRebuild.Add(TutorialTileMapAsset.Object);*/
	// Chaff
	static ConstructorHelpers::FObjectFinder<UPaperTileMap>Filler_TileMapAsset(TEXT("PaperTileMap'/Game/Tilesets/Filler_TileMap.Filler_TileMap'"));
	TileMapsToRebuild.Add(Filler_TileMapAsset.Object);
	static ConstructorHelpers::FObjectFinder<UPaperTileMap>LavaFiller_TileMapAsset(TEXT("PaperTileMap'/Game/Tilesets/LavaFiller_TileMap.LavaFiller_TileMap'"));
	TileMapsToRebuild.Add(LavaFiller_TileMapAsset.Object);
	static ConstructorHelpers::FObjectFinder<UPaperTileMap>TestLevel_TileMapAsset(TEXT("PaperTileMap'/Game/Tilesets/TestLevel_TileMap.TestLevel_TileMap'"));
	TileMapsToRebuild.Add(TestLevel_TileMapAsset.Object);
	// Platforms
	static ConstructorHelpers::FObjectFinder<UPaperTileMap>LongPlatform_TileMapAsset(TEXT("PaperTileMap'/Game/Tilesets/Platforms/LongPlatform_TileMap.LongPlatform_TileMap'"));
	TileMapsToRebuild.Add(LongPlatform_TileMapAsset.Object);
	static ConstructorHelpers::FObjectFinder<UPaperTileMap>MidPlatform_TileMapAsset(TEXT("PaperTileMap'/Game/Tilesets/Platforms/MidPlatform_TileMap.MidPlatform_TileMap'"));
	TileMapsToRebuild.Add(MidPlatform_TileMapAsset.Object);
	static ConstructorHelpers::FObjectFinder<UPaperTileMap>MovingPlatform_TileMapAsset(TEXT("PaperTileMap'/Game/Tilesets/Platforms/MovingPlatform_TileMap.MovingPlatform_TileMap'"));
	TileMapsToRebuild.Add(MovingPlatform_TileMapAsset.Object);
	static ConstructorHelpers::FObjectFinder<UPaperTileMap>ShortPlatform_TileMapAsset(TEXT("PaperTileMap'/Game/Tilesets/Platforms/ShortPlatform_TileMap.ShortPlatform_TileMap'"));
	TileMapsToRebuild.Add(ShortPlatform_TileMapAsset.Object);


	UGameplayStatics::DoesSaveGameExist("SettingsSlot", 0) ?
		OptionsSaveGame = Cast<UOptionsSaveGame>(UGameplayStatics::LoadGameFromSlot("SettingsSlot", 0)) :
		OptionsSaveGame = Cast<UOptionsSaveGame>(UGameplayStatics::CreateSaveGameObject(UOptionsSaveGame::StaticClass()));

	WindowMode = EWindowMode::Type::NumWindowModes;
	bEnableSoundCompass = false;
	bEnableHighVisMode = false;
	MasterVolume = -1.f;
	GameVolume = -1.f;
	MusicVolume = -1.f;
	AmbientVolume = -1.f;
	bSettingsChanged = false;

	bRecordingActionInput = false;
	bRecordingAxisInput = false;
	AxisScale = 0.f;
}

void UOptionsEditWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UGameplayStatics::DoesSaveGameExist("SettingsSlot", 0) ?
		OptionsSaveGame = Cast<UOptionsSaveGame>(UGameplayStatics::LoadGameFromSlot("SettingsSlot", 0)) :
		OptionsSaveGame = Cast<UOptionsSaveGame>(UGameplayStatics::CreateSaveGameObject(UOptionsSaveGame::StaticClass()));

	WindowMode = OptionsSaveGame->GetWindowMode();
	bEnableSoundCompass = OptionsSaveGame->GetEnableSoundCompass();
	bEnableHighVisMode = OptionsSaveGame->GetEnableHighVisMode();
	MasterVolume = OptionsSaveGame->GetMasterVolume();
	GameVolume = OptionsSaveGame->GetGameVolume();
	MusicVolume = OptionsSaveGame->GetMusicVolume();
	AmbientVolume = OptionsSaveGame->GetAmbientVolume();

	AudioDevice->SetRTPCValue(*FString("MasterVolume"), MasterVolume, 0.f, nullptr);
	AudioDevice->SetRTPCValue(*FString("GameVolume"), GameVolume, 0.f, nullptr);
	AudioDevice->SetRTPCValue(*FString("MusicVolume"), MusicVolume, 0.f, nullptr);
	AudioDevice->SetRTPCValue(*FString("AmbientVolume"), AmbientVolume, 0.f, nullptr);
	UGameUserSettings::GetGameUserSettings()->SetFullscreenMode(WindowMode.GetValue());
	UGameUserSettings::GetGameUserSettings()->ApplyResolutionSettings(false);
	if (!UGameUserSettings::GetGameUserSettings()->IsVSyncEnabled())
		UGameUserSettings::GetGameUserSettings()->SetVSyncEnabled(true);

	if (!bEnableHighVisMode)
		LevelTileSet->SetTileSheetTexture(BaseTileSetTexture);
	else if (bEnableHighVisMode)
		LevelTileSet->SetTileSheetTexture(HighVisTileSetTexture);
	UpdateTileMaps();
	UpdateFlipbookActors();
	UpdateSprites();

	//ItemStyle.SetHovered(UWidgetBlueprintLibrary::MakeBrushFromTexture(SelectedItemBackplate, 61.f, 61.f));
	//ItemStyle.SetPressed(UWidgetBlueprintLibrary::MakeBrushFromTexture(ItemBackplate, 61.f, 61.f));
}

void UOptionsEditWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (HasKeyboardFocus()) {
		if (DefaultFocusWidget->IsVisible())
			DefaultFocusWidget->SetKeyboardFocus();
		else if (SettingsPanelWidget && ControlsPanelWidget) {
			if (SettingsPanelWidget->IsVisible())
				SettingsFocusWidget->SetKeyboardFocus();
			else if (ControlsPanelWidget->IsVisible())
				ControlsFocusWidget->SetKeyboardFocus();
		}
	}

	UpdateSetting();

	if (bSettingsChanged)
		SaveAudioSettings();
}

FReply UOptionsEditWidget::NativeOnPreviewKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	if (bRecordingActionInput) {
		InternalActionRebind(InKeyEvent.GetKey());
		return FReply::Handled();
	}
	else if (bRecordingAxisInput) {
		InternalAxisRebind(InKeyEvent.GetKey());
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

FReply UOptionsEditWidget::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (bRecordingActionInput) {
		InternalActionRebind(InMouseEvent.GetEffectingButton());
		return FReply::Handled();
	}
	else if (bRecordingAxisInput) {
		InternalAxisRebind(InMouseEvent.GetEffectingButton());
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

FReply UOptionsEditWidget::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (bRecordingActionInput) {
		if (InMouseEvent.GetWheelDelta() < 0)
			InternalActionRebind(FKey(FName("Mouse Wheel Down")));
		else if (InMouseEvent.GetWheelDelta() > 0)
			InternalActionRebind(FKey(FName("Mouse Wheel Up")));

		return FReply::Handled();
	}
	else if (bRecordingAxisInput) {
		if (InMouseEvent.GetWheelDelta() < 0)
			InternalAxisRebind(FKey(FName("Mouse Wheel Down")));
		else if (InMouseEvent.GetWheelDelta() > 0)
			InternalAxisRebind(FKey(FName("Mouse Wheel Up")));

		return FReply::Handled();
	}
	return FReply::Unhandled();
}

void UOptionsEditWidget::SetWidgets(USlider* InMasterSlider, USlider* InGameSlider, USlider* InMusicSlider, USlider* InAmbientSlider,
	UCheckBox* InSoundCompassCheckBox, UCheckBox* InHighVisCheckBox, UComboBoxString* InWindowModeBox)
{
	MasterSlider = InMasterSlider;
	if (MasterSlider)
		MasterSlider->SetValue(OptionsSaveGame->GetMasterVolume());

	GameSlider = InGameSlider;
	if (GameSlider)
		GameSlider->SetValue(OptionsSaveGame->GetGameVolume());

	MusicSlider = InMusicSlider;
	if (MusicSlider)
		MusicSlider->SetValue(OptionsSaveGame->GetMusicVolume());

	AmbientSlider = InAmbientSlider;
	if (AmbientSlider)
		AmbientSlider->SetValue(OptionsSaveGame->GetAmbientVolume());

	SoundCompassCheckBox = InSoundCompassCheckBox;
	if (SoundCompassCheckBox) {
		if (OptionsSaveGame->GetEnableSoundCompass()) {
			SoundCompassCheckBox->SetCheckedState(ECheckBoxState::Checked);
		}
		else {
			SoundCompassCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
		}
	}

	HighVisCheckBox = InHighVisCheckBox;
	if (HighVisCheckBox) {
		if (OptionsSaveGame->GetEnableHighVisMode()) {
			HighVisCheckBox->SetCheckedState(ECheckBoxState::Checked);
		}
		else {
			HighVisCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
		}
	}


	WindowModeBox = InWindowModeBox;
	if (WindowModeBox)
		WindowModeBox->SetSelectedIndex(OptionsSaveGame->GetWindowMode().GetValue());
}

void UOptionsEditWidget::UpdateSetting()
{
	float NewVol;
	if (MasterSlider) {
		NewVol = MasterSlider->GetValue();
		if (NewVol != MasterVolume) {
			MasterVolume = NewVol;
			bSettingsChanged = true;
			AudioDevice->SetRTPCValue(*FString("MasterVolume"), MasterVolume, 0.f, nullptr);
		}
	}

	if (GameSlider) {
		NewVol = GameSlider->GetValue();
		if (NewVol != GameVolume) {
			GameVolume = NewVol;
			bSettingsChanged = true;
			AudioDevice->SetRTPCValue(*FString("GameVolume"), GameVolume, 0.f, nullptr);
		}
	}

	if (MusicSlider) {
		NewVol = MusicSlider->GetValue();
		if (NewVol != MusicVolume) {
			MusicVolume = NewVol;
			bSettingsChanged = true;
			AudioDevice->SetRTPCValue(*FString("MusicVolume"), MusicVolume, 0.f, nullptr);
		}
	}

	if (AmbientSlider) {
		NewVol = AmbientSlider->GetValue();
		if (NewVol != AmbientVolume) {
			AmbientVolume = NewVol;
			bSettingsChanged = true;
			AudioDevice->SetRTPCValue(*FString("AmbientVolume"), AmbientVolume, 0.f, nullptr);
		}
	}

	ECheckBoxState NewCheckState;
	if (SoundCompassCheckBox) {
		NewCheckState = SoundCompassCheckBox->GetCheckedState();
		if (bEnableSoundCompass && NewCheckState == ECheckBoxState::Unchecked) {
			bEnableSoundCompass = false;
			bSettingsChanged = true;
		}
		else if (!bEnableSoundCompass && NewCheckState == ECheckBoxState::Checked) {
			bEnableSoundCompass = true;
			bSettingsChanged = true;
		}
	}

	if (HighVisCheckBox) {
		NewCheckState = HighVisCheckBox->GetCheckedState();
		if (bEnableHighVisMode && NewCheckState == ECheckBoxState::Unchecked) {
			bEnableHighVisMode = false;
			bSettingsChanged = true;

			LevelTileSet->SetTileSheetTexture(BaseTileSetTexture);
			UpdateTileMaps();
			UpdateFlipbookActors();
			UpdateSprites();
		}
		else if (!bEnableHighVisMode && NewCheckState == ECheckBoxState::Checked) {
			bEnableHighVisMode = true;
			bSettingsChanged = true;

			LevelTileSet->SetTileSheetTexture(HighVisTileSetTexture);
			UpdateTileMaps();
			UpdateFlipbookActors();
			UpdateSprites();
		}
	}

	if (WindowModeBox) {
		TEnumAsByte<EWindowMode::Type> NewMode = TEnumAsByte<EWindowMode::Type>(WindowModeBox->GetSelectedIndex());
		if (NewMode != WindowMode) {
			WindowMode = NewMode;
			bSettingsChanged = true;
			UGameUserSettings::GetGameUserSettings()->SetFullscreenMode(WindowMode.GetValue());
			UGameUserSettings::GetGameUserSettings()->ApplyResolutionSettings(false);
		}
	}
}

void UOptionsEditWidget::SaveAudioSettings()
{
	UGameplayStatics::DoesSaveGameExist("SettingsSlot", 0) ?
		OptionsSaveGame = Cast<UOptionsSaveGame>(UGameplayStatics::LoadGameFromSlot("SettingsSlot", 0)) :
		OptionsSaveGame = Cast<UOptionsSaveGame>(UGameplayStatics::CreateSaveGameObject(UOptionsSaveGame::StaticClass()));

	OptionsSaveGame->SetAudioOptions(bEnableSoundCompass, bEnableHighVisMode, MasterVolume, GameVolume, MusicVolume, AmbientVolume, WindowMode);
	UGameplayStatics::SaveGameToSlot(OptionsSaveGame, "SettingsSlot", 0);
	bSettingsChanged = false;
}

void UOptionsEditWidget::UpdateTileMaps()
{
	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), AllActors);
	for (AActor* Actor : AllActors) {
		TArray<UActorComponent*>MapComps = Actor->GetComponentsByClass(UPaperTileMapComponent::StaticClass());

		for (UActorComponent* Comp : MapComps) {
			UPaperTileMap* NewMap = Cast<UPaperTileMapComponent>(Comp)->TileMap->CloneTileMap(Comp);
			Cast<UPaperTileMapComponent>(Comp)->SetTileMap(NewMap);
		}
	}
}

void UOptionsEditWidget::UpdateFlipbookActors()
{
	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), AllActors);
	for (AActor* Actor : AllActors) {
		ACharacterBase* Character = Cast<ACharacterBase>(Actor);
		if (Character)
			Character->UseAltAnims(bEnableHighVisMode);
	}
}

void UOptionsEditWidget::UpdateSprites()
{
	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMyPaperSpriteActor::StaticClass(), AllActors);
	for (AActor* Actor : AllActors) {
		Cast<AMyPaperSpriteActor>(Actor)->UseAltSprite(bEnableHighVisMode);
	}
}

void UOptionsEditWidget::InternalActionRebind(FKey Key)
{
	UInputSettings* InputSettings = UInputSettings::GetInputSettings();

	TArray<FInputActionKeyMapping>Mappings;
	InputSettings->GetActionMappingByName(RecordingMappingName, Mappings);

	for (FInputActionKeyMapping Mapping : Mappings) {
		if (Mapping.Key.IsGamepadKey() && Key.IsGamepadKey() && bRecordingGamepad)
			InputSettings->RemoveActionMapping(Mapping, true);
		else if (!Mapping.Key.IsGamepadKey() && !Key.IsGamepadKey() && !bRecordingGamepad)
			InputSettings->RemoveActionMapping(Mapping, true);
	}

	bool bRecordInput = (Key.IsGamepadKey() && bRecordingGamepad) || (!Key.IsGamepadKey() && !bRecordingGamepad);
	if (bRecordInput)
		InputSettings->AddActionMapping(FInputActionKeyMapping(RecordingMappingName, Key), true);

	RecordingMappingName = FName();
	bRecordingActionInput = false;
	bRecordingGamepad = false;

	if (RecordingWidget)
		RecordingWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UOptionsEditWidget::InternalAxisRebind(FKey Key)
{
	UInputSettings* InputSettings = UInputSettings::GetInputSettings();

	TArray<FInputAxisKeyMapping>Mappings;
	InputSettings->GetAxisMappingByName(RecordingMappingName, Mappings);

	for (FInputAxisKeyMapping Mapping : Mappings) {
		if (Mapping.Key.IsGamepadKey() && Key.IsGamepadKey() && bRecordingGamepad)
			InputSettings->RemoveAxisMapping(Mapping, true);
		else if (!Mapping.Key.IsGamepadKey() && !Key.IsGamepadKey() && !bRecordingGamepad)
			if (Mapping.Scale == AxisScale)
				InputSettings->RemoveAxisMapping(Mapping, true);
	}

	bool bRecordInput = (Key.IsGamepadKey() && bRecordingGamepad) || (!Key.IsGamepadKey() && !bRecordingGamepad);

	bool bLeftJoystick =
		(Key == FKey(FName("Gamepad_LeftStick_Up"))
			|| Key == FKey(FName("Gamepad_LeftStick_Down"))
			|| Key == FKey(FName("Gamepad_LeftStick_Right"))
			|| Key == FKey(FName("Gamepad_LeftStick_Left"))
			|| Key == FKey(FName("Gamepad_LeftX"))
			|| Key == FKey(FName("GGamepad_LeftY")));

	bool bRightJoystick =
		(Key == FKey(FName("Gamepad_RightStick_Up"))
			|| Key == FKey(FName("Gamepad_RightStick_Down"))
			|| Key == FKey(FName("Gamepad_RightStick_Right"))
			|| Key == FKey(FName("Gamepad_RightStick_Left"))
			|| Key == FKey(FName("Gamepad_RightX"))
			|| Key == FKey(FName("GGamepad_RightY")));

	if (bRecordInput) {
		if (bLeftJoystick)
			InputSettings->AddAxisMapping(FInputAxisKeyMapping(RecordingMappingName, FKey(FName("Gamepad_Left2D")), AxisScale), true);
		else if (bRightJoystick)
			InputSettings->AddAxisMapping(FInputAxisKeyMapping(RecordingMappingName, FKey(FName("Gamepad_Right2D")), AxisScale), true);
		else {
			InputSettings->AddAxisMapping(FInputAxisKeyMapping(RecordingMappingName, Key, AxisScale), true);
		}
	}

	RecordingMappingName = FName();
	bRecordingAxisInput = false;
	bRecordingGamepad = false;
	AxisScale = 0.f;

	if (RecordingWidget)
		RecordingWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UOptionsEditWidget::RebindMKAction(FName InActionName)
{
	if (RecordingWidget)
		RecordingWidget->SetVisibility(ESlateVisibility::Visible);

	RecordingMappingName = InActionName;
	bRecordingActionInput = true;
	bRecordingGamepad = false;
}

void UOptionsEditWidget::RebindGPAction(FName InActionName)
{
	if (RecordingWidget)
		RecordingWidget->SetVisibility(ESlateVisibility::Visible);

	RecordingMappingName = InActionName;
	bRecordingActionInput = true;
	bRecordingGamepad = true;
}

void UOptionsEditWidget::RebindMKAxis(FName InAxisName, float Scale)
{
	if (RecordingWidget)
		RecordingWidget->SetVisibility(ESlateVisibility::Visible);

	RecordingMappingName = InAxisName;
	bRecordingAxisInput = true;
	bRecordingGamepad = false;
	AxisScale = Scale;
}

void UOptionsEditWidget::RebindGPAxis(FName InAxisName, float Scale)
{
	if (RecordingWidget)
		RecordingWidget->SetVisibility(ESlateVisibility::Visible);

	RecordingMappingName = InAxisName;
	bRecordingAxisInput = true;
	bRecordingGamepad = true;
	AxisScale = Scale;
}

FText UOptionsEditWidget::GetMKAction(FName InActionName)
{
	UInputSettings* InputSettings = UInputSettings::GetInputSettings();
	TArray<FInputActionKeyMapping>Mappings;
	InputSettings->GetActionMappingByName(InActionName, Mappings);
	for (FInputActionKeyMapping Mapping : Mappings) {
		if (!Mapping.Key.IsGamepadKey())
			return Mapping.Key.GetDisplayName();
	}
	return FText::FromString("Null");
}

FText UOptionsEditWidget::GetGPAction(FName InActionName)
{
	UInputSettings* InputSettings = UInputSettings::GetInputSettings();
	TArray<FInputActionKeyMapping>Mappings;
	InputSettings->GetActionMappingByName(InActionName, Mappings);
	for (FInputActionKeyMapping Mapping : Mappings) {
		if (Mapping.Key.IsGamepadKey())
			return Mapping.Key.GetDisplayName();
	}
	return FText::FromString("Null");
}

FText UOptionsEditWidget::GetMKAxis(FName InAxisName, float Scale)
{
	UInputSettings* InputSettings = UInputSettings::GetInputSettings();
	TArray<FInputAxisKeyMapping>Mappings;
	InputSettings->GetAxisMappingByName(InAxisName, Mappings);
	for (FInputAxisKeyMapping Mapping : Mappings) {
		if (!Mapping.Key.IsGamepadKey())
			if (Mapping.Scale == Scale)
				return Mapping.Key.GetDisplayName();
	}
	return FText::FromString("Null");
}

FText UOptionsEditWidget::GetGPAxis(FName InAxisName, float Scale)
{
	UInputSettings* InputSettings = UInputSettings::GetInputSettings();
	TArray<FInputAxisKeyMapping>Mappings;
	InputSettings->GetAxisMappingByName(InAxisName, Mappings);
	for (FInputAxisKeyMapping Mapping : Mappings) {
		if (Mapping.Key.IsGamepadKey())
			if (Mapping.Scale == Scale)
				return Mapping.Key.GetDisplayName();
	}
	return FText::FromString("Null");
}
