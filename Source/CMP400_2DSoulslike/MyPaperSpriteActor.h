// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PaperSpriteActor.h"
#include "MyPaperSpriteActor.generated.h"

UCLASS()
class CMP400_2DSOULSLIKE_API AMyPaperSpriteActor : public APaperSpriteActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sprite)
		class UPaperSprite* BaseSprite;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sprite)
		class UPaperSprite* AltSprite;

public:
	AMyPaperSpriteActor();

protected:
	virtual void BeginPlay() override;

public:
	void UseAltSprite(bool Val);

};
