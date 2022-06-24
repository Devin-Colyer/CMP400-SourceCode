// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TargetedAttack.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "CharacterBase.h"

UBTTask_TargetedAttack::UBTTask_TargetedAttack()
{
	SelfKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_TargetedAttack, SelfKey), AActor::StaticClass());
	TargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_TargetedAttack, TargetKey), AActor::StaticClass());
}

void UBTTask_TargetedAttack::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	SelfKey.ResolveSelectedKey(*Asset.GetBlackboardAsset());
	TargetKey.ResolveSelectedKey(*Asset.GetBlackboardAsset());
}

EBTNodeResult::Type UBTTask_TargetedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get self
	ACharacterBase* Self = Cast<ACharacterBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(SelfKey.SelectedKeyName));
	Self->AITargetedAttackDelegate.Broadcast(Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName)));

	return EBTNodeResult::Succeeded;
}