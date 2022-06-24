// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ExecutionerCheckAbilities.generated.h"

UCLASS()
class CMP400_2DSOULSLIKE_API UBTTask_ExecutionerCheckAbilities : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector SelfKey;
	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector SummonKey;
	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector HealKey;

public:
	UBTTask_ExecutionerCheckAbilities();

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
