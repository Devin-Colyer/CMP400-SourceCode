// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ExecutionerCheckAbilities.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Enemy_BossSummoner.h"

UBTTask_ExecutionerCheckAbilities::UBTTask_ExecutionerCheckAbilities()
{
	SelfKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_ExecutionerCheckAbilities, SelfKey), AActor::StaticClass());
	SummonKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_ExecutionerCheckAbilities, SummonKey));
	HealKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_ExecutionerCheckAbilities, HealKey));
}

void UBTTask_ExecutionerCheckAbilities::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	SelfKey.ResolveSelectedKey(*Asset.GetBlackboardAsset());
	SummonKey.ResolveSelectedKey(*Asset.GetBlackboardAsset());
	HealKey.ResolveSelectedKey(*Asset.GetBlackboardAsset());
}

EBTNodeResult::Type UBTTask_ExecutionerCheckAbilities::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemy_BossSummoner* Self = Cast<AEnemy_BossSummoner>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(SelfKey.SelectedKeyName));

	if (Self) {
		if (Self->CanSummon()) {
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(SummonKey.SelectedKeyName, true);
		}
		else {
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(SummonKey.SelectedKeyName, false);
		}

		if (Self->CanHeal()) {
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(HealKey.SelectedKeyName, true);
		}
		else {
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(HealKey.SelectedKeyName, false);
		}

		return EBTNodeResult::Succeeded;
	}
	else {
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(SummonKey.SelectedKeyName, false);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(HealKey.SelectedKeyName, false);
		return EBTNodeResult::Failed;
	}
}
