// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_IsHealthBelowPercentage.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "CharacterBase.h"

UBTTask_IsHealthBelowPercentage::UBTTask_IsHealthBelowPercentage()
{
	Percentage = 0.5f;

	SelfKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_IsHealthBelowPercentage, SelfKey), AActor::StaticClass());
	ResultKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_IsHealthBelowPercentage, ResultKey));
}

void UBTTask_IsHealthBelowPercentage::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	SelfKey.ResolveSelectedKey(*Asset.GetBlackboardAsset());
	ResultKey.ResolveSelectedKey(*Asset.GetBlackboardAsset());
}

EBTNodeResult::Type UBTTask_IsHealthBelowPercentage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get self
	ACharacterBase* Self = Cast<ACharacterBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(SelfKey.SelectedKeyName));
	Percentage = FMath::Clamp(Percentage, 0.f, 1.f);

	if (Self) {
		if (Self->GetHealth() <= Percentage) {
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
