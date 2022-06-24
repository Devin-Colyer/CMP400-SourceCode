// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TargetedAttack.generated.h"

UCLASS()
class CMP400_2DSOULSLIKE_API UBTTask_TargetedAttack : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector SelfKey;
	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector TargetKey;

public:
	UBTTask_TargetedAttack();

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
