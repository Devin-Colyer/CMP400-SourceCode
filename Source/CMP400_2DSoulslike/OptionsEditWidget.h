// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "OptionsEditWidget.generated.h"

class USlider;
class UPaperTileMap;
class UCheckBox;
class UComboBox;
class UAkRtpc;

UCLASS()
class CMP400_2DSOULSLIKE_API UOptionsEditWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite)
		UWidget* DefaultFocusWidget;
	UPROPERTY(BlueprintReadWrite)
		UWidget* SettingsFocusWidget;
	UPROPERTY(BlueprintReadWrite)
		UWidget* SettingsPanelWidget;
	UPROPERTY(BlueprintReadWrite)
		UWidget* ControlsFocusWidget;
	UPROPERTY(BlueprintReadWrite)
		UWidget* ControlsPanelWidget;

	UPROPERTY(BlueprintReadWrite)
		UWidget* RecordingWidget;

	//UPROPERTY(BlueprintReadWrite)
	//	FButtonStyle ItemStyle;
	//UPROPERTY(BlueprintReadWrite)
	//	TArray<class UInputKeySelector*> ButtonArr;

	// Audio
	class FAkAudioDevice* AudioDevice;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Volume)
		UAkRtpc* MasterVolumeParam;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Volume)
		UAkRtpc* GameVolumeParam;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Volume)
		UAkRtpc* MusicVolumeParam;

	// UCheckbox* EnableSoundCompassCheckbox
	USlider* MasterSlider;
	USlider* MusicSlider;
	USlider* GameSlider;
	USlider* AmbientSlider;
	UCheckBox* SoundCompassCheckBox;
	UCheckBox* HighVisCheckBox;
	UComboBoxString* WindowModeBox;

	TEnumAsByte<EWindowMode::Type> WindowMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HighVis)
		bool bEnableHighVisMode;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HighVis)
		class UPaperTileSet* LevelTileSet;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HighVis)
		UTexture2D* BaseTileSetTexture;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HighVis)
		UTexture2D* HighVisTileSetTexture;
	TArray<UPaperTileMap*> TileMapsToRebuild;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Audio)
		bool bEnableSoundCompass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Audio)
		float MasterVolume;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Audio)
		float GameVolume;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Audio)
		float MusicVolume;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Audio)
		float AmbientVolume;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Audio)
		bool bSettingsChanged;

	// Controls
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Controls)
		bool bRecordingActionInput;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Controls)
		bool bRecordingAxisInput;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Controls)
		bool bRecordingGamepad;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Controls)
		FName RecordingMappingName;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Controls)
		float AxisScale;

	class UOptionsSaveGame* OptionsSaveGame;

public:
	UOptionsEditWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UFUNCTION(BlueprintCallable)
		void SetWidgets(USlider* InMasterSlider, USlider* InGameSlider, USlider* InMusicSlider, USlider* InAmbientSlider,
			UCheckBox* InSoundCompassCheckBox, UCheckBox* InHighVisCheckBox, UComboBoxString* InWindowModeBox);
	void UpdateSetting();
	void SaveAudioSettings();

	// Internal reset for tilemaps
	void UpdateTileMaps();
	void UpdateFlipbookActors();
	void UpdateSprites();

	// Internal Rebinds - Handles specific mappings
	void InternalActionRebind(FKey Key);
	void InternalAxisRebind(FKey Key);

	// Rebind Actions
	UFUNCTION(BlueprintCallable)
		void RebindMKAction(FName InActionName);
	UFUNCTION(BlueprintCallable)
		void RebindGPAction(FName InActionName);

	// Rebind Axes
	UFUNCTION(BlueprintCallable)
		void RebindMKAxis(FName InAxisName, float Scale = 1.f);
	UFUNCTION(BlueprintCallable)
		void RebindGPAxis(FName InAxisName, float Scale = 1.f);

	// Get Action Display
	UFUNCTION(BlueprintPure)
		FText GetMKAction(FName InActionName);
	UFUNCTION(BlueprintPure)
		FText GetGPAction(FName InActionName);

	// Get Axis Display
	UFUNCTION(BlueprintPure)
		FText GetMKAxis(FName InAxisName, float Scale = 1.f);
	UFUNCTION(BlueprintPure)
		FText GetGPAxis(FName InAxisName, float Scale = 1.f);

public:

	
};
