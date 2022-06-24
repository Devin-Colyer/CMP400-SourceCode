// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_CheckPlayerInRange.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_CheckPlayerInRange::UBTTask_CheckPlayerInRange()
{
	SearchRadius = 500.f;

	SelfKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_CheckPlayerInRange, SelfKey), AActor::StaticClass());
	TargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_CheckPlayerInRange, TargetKey), AActor::StaticClass());
	ResultKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_CheckPlayerInRange, ResultKey));
}

void UBTTask_CheckPlayerInRange::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	SelfKey.ResolveSelectedKey(*Asset.GetBlackboardAsset());
	TargetKey.ResolveSelectedKey(*Asset.GetBlackboardAsset());
	ResultKey.ResolveSelectedKey(*Asset.GetBlackboardAsset());
}

EBTNodeResult::Type UBTTask_CheckPlayerInRange::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get self
	AActor* Self = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(SelfKey.SelectedKeyName));
	// Get the player
	AActor* Player = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName));

	// Set AI's target location to the new location
	if (Player) {
		if (Self->GetDistanceTo(Player) <= SearchRadius) {
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(ResultKey.SelectedKeyName, true);
		}
		else {
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(ResultKey.SelectedKeyName, false);
		}

		return EBTNodeResult::Succeeded;
	}
	else {
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(ResultKey.SelectedKeyName, false);
		return EBTNodeResult::Failed;
	}
}