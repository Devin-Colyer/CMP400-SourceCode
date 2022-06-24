// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AttackNoParam.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "CharacterBase.h"

UBTTask_AttackNoParam::UBTTask_AttackNoParam()
{
	SelfKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_AttackNoParam, SelfKey), AActor::StaticClass());
}

void UBTTask_AttackNoParam::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	SelfKey.ResolveSelectedKey(*Asset.GetBlackboardAsset());
}

EBTNodeResult::Type UBTTask_AttackNoParam::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get self
	ACharacterBase* Self = Cast<ACharacterBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(SelfKey.SelectedKeyName));
	Self->AIAttackDelegateNoParam.Broadcast();

	return EBTNodeResult::Succeeded;
}
