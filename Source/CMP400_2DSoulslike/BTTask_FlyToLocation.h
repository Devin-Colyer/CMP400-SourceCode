// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FlyToLocation.generated.h"

UCLASS()
class CMP400_2DSOULSLIKE_API UBTTask_FlyToLocation : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector SelfKey;
	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector TargetKey;
	UPROPERTY(EditAnywhere)
		float AcceptanceRadius;
	UPROPERTY(EditAnywhere)
		bool bTrackMovingGoal;
	UPROPERTY(EditAnywhere)
		bool bUseLerp;
	UPROPERTY(EditAnywhere)
		float FlightSpeed;

	ACharacter* Self;
	FVector InitLoc;
	FVector TargetLoc;
	float InitDist;

public:
	UBTTask_FlyToLocation();

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	
};
