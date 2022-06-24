// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_IsHealthBelowPercentage.generated.h"

UCLASS()
class CMP400_2DSOULSLIKE_API UBTTask_IsHealthBelowPercentage : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector SelfKey;
	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector ResultKey;
	UPROPERTY(EditAnywhere, meta = (ToolTip = "Percentage of health remaining. 0 <= x <= 1"))
		float Percentage;

public:
	UBTTask_IsHealthBelowPercentage();

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
