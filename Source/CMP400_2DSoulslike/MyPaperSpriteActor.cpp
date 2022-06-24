// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPaperSpriteActor.h"

// Kismet
#include "Kismet/GameplayStatics.h"

// Paper 2D
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"

// My headers
#include "OptionsSaveGame.h"


AMyPaperSpriteActor::AMyPaperSpriteActor()
{
	static ConstructorHelpers::FObjectFinder<UPaperSprite>BaseSpriteAsset(TEXT("PaperSprite'/Game/Textures/AICON-Green_Sprite_0.AICON-Green_Sprite_0'"));
	BaseSprite = BaseSpriteAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperSprite>AltSpriteAsset(TEXT("PaperSprite'/Game/Textures/AICON-Red_Sprite_0.AICON-Red_Sprite_0'"));
	AltSprite = AltSpriteAsset.Object;
}

void AMyPaperSpriteActor::BeginPlay()
{
	Super::BeginPlay();

	UOptionsSaveGame* OptionsSaveGame;
	UGameplayStatics::DoesSaveGameExist("SettingsSlot", 0) ?
		OptionsSaveGame = Cast<UOptionsSaveGame>(UGameplayStatics::LoadGameFromSlot("SettingsSlot", 0)) :
		OptionsSaveGame = Cast<UOptionsSaveGame>(UGameplayStatics::CreateSaveGameObject(UOptionsSaveGame::StaticClass()));
	UseAltSprite(OptionsSaveGame->GetEnableHighVisMode());
}

void AMyPaperSpriteActor::UseAltSprite(bool Val)
{
	if (Val) {
		GetRenderComponent()->SetSprite(AltSprite);
	}
	else {
		GetRenderComponent()->SetSprite(BaseSprite);
	}
}
