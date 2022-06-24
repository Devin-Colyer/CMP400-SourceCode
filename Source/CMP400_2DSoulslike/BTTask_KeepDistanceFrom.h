// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_KeepDistanceFrom.generated.h"

UCLASS()
class CMP400_2DSOULSLIKE_API UBTTask_KeepDistanceFrom : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector SelfKey;
	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector TargetKey;
	UPROPERTY(EditAnywhere)
		float Radius;

public:
	UBTTask_KeepDistanceFrom();

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
