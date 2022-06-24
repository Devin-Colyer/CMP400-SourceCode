// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_StopAttack.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "CharacterBase.h"

UBTTask_StopAttack::UBTTask_StopAttack()
{
	SelfKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_StopAttack, SelfKey), AActor::StaticClass());
}

void UBTTask_StopAttack::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	SelfKey.ResolveSelectedKey(*Asset.GetBlackboardAsset());
}

EBTNodeResult::Type UBTTask_StopAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get self
	ACharacterBase* Self = Cast<ACharacterBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(SelfKey.SelectedKeyName));
	Self->AIStopAttackDelegate.Broadcast();

	return EBTNodeResult::Succeeded;
}

