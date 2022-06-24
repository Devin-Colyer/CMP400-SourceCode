// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TurnAround.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTTask_TurnAround::UBTTask_TurnAround()
{
	SelfKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_TurnAround, SelfKey), AActor::StaticClass());
}

void UBTTask_TurnAround::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	SelfKey.ResolveSelectedKey(*Asset.GetBlackboardAsset());
}

EBTNodeResult::Type UBTTask_TurnAround::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* SelfController = OwnerComp.GetAIOwner();
	if (!SelfController)
		return EBTNodeResult::Failed;

	FRotator NewRot = SelfController->GetControlRotation();
	NewRot.Yaw += 180;
	SelfController->SetControlRotation(NewRot);

	return EBTNodeResult::Succeeded;
}
