// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CheckFacingPlayer.generated.h"

UCLASS()
class CMP400_2DSOULSLIKE_API UBTTask_CheckFacingPlayer : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector SelfKey;
	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector TargetKey;
	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector ResultKey;

public:
	UBTTask_CheckFacingPlayer();

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
