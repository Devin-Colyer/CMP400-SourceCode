// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindFlyingLocationInRange.generated.h"

UCLASS()
class CMP400_2DSOULSLIKE_API UBTTask_FindFlyingLocationInRange : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector SelfKey;
	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector ResultKey;
	UPROPERTY(EditAnywhere)
		float MinDistance;
	UPROPERTY(EditAnywhere)
		float Radius;

	FVector GetNewLocation(AActor* Self);

public:
	UBTTask_FindFlyingLocationInRange();

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
