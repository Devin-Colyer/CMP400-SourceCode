// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PassThroughPlatform.h"
#include "MovingPlatform.generated.h"

UCLASS()
class CMP400_2DSOULSLIKE_API AMovingPlatform : public APassThroughPlatform
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = Movement)
		float PlatformSpeed;
	UPROPERTY(EditAnywhere, Category = Movement)
		FVector StartingLocation;
	UPROPERTY(EditAnywhere, Category = Movement)
		FVector TargetLocation;

	UPROPERTY(EditDefaultsOnly)
		class UBoxComponent* StickyBox;

	UPROPERTY(VisibleAnywhere, Category = Movement)
	float PlatformProgress;

public:
	AMovingPlatform();

protected:
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

public:
	
};
