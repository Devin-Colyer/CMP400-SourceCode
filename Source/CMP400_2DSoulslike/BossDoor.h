// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BossDoor.generated.h"

UCLASS()
class CMP400_2DSOULSLIKE_API ABossDoor : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = TileMap)
		class UPaperTileMapComponent* DoorMapComp;
	class UPaperTileMap* DeafultDoorMap;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TileMap)
		class UPaperTileMap* DoorMap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Volume)
		class UBoxComponent* BossVolume;

	bool bBossDetected;
	
public:	
	// Sets default values for this actor's properties
	ABossDoor();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
