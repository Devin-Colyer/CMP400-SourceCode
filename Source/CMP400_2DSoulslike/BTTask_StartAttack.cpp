// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_StartAttack.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "CharacterBase.h"

UBTTask_StartAttack::UBTTask_StartAttack()
{
	SelfKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_StartAttack, SelfKey), AActor::StaticClass());
}

void UBTTask_StartAttack::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	SelfKey.ResolveSelectedKey(*Asset.GetBlackboardAsset());
}

EBTNodeResult::Type UBTTask_StartAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get self
	ACharacterBase* Self = Cast<ACharacterBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(SelfKey.SelectedKeyName));
	Self->AIStartAttackDelegate.Broadcast();

	return EBTNodeResult::Succeeded;
}

