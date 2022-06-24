// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "CharacterBase.h"

UBTTask_Attack::UBTTask_Attack()
{
	AttackIndex = -1;
	SelfKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_Attack, SelfKey), AActor::StaticClass());
}

void UBTTask_Attack::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	SelfKey.ResolveSelectedKey(*Asset.GetBlackboardAsset());
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get self
	ACharacterBase* Self = Cast<ACharacterBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(SelfKey.SelectedKeyName));
	Self->AIAttackDelegate.Broadcast(AttackIndex);

	return EBTNodeResult::Succeeded;
}