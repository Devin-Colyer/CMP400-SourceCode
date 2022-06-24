// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PassThroughPlatform.generated.h"

UCLASS()
class CMP400_2DSOULSLIKE_API APassThroughPlatform : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		class UPaperTileMapComponent* Map;
	UPROPERTY(EditDefaultsOnly)
		class UBoxComponent* TriggerBox;

public:	
	// Sets default values for this actor's properties
	APassThroughPlatform();

protected:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:

};
