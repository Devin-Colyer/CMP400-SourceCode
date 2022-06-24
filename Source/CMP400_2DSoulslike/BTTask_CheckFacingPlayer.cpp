// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_CheckFacingPlayer.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_CheckFacingPlayer::UBTTask_CheckFacingPlayer()
{
	SelfKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_CheckFacingPlayer, SelfKey), AActor::StaticClass());
	TargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_CheckFacingPlayer, TargetKey), AActor::StaticClass());
	ResultKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_CheckFacingPlayer, ResultKey));
}

void UBTTask_CheckFacingPlayer::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	SelfKey.ResolveSelectedKey(*Asset.GetBlackboardAsset());
	TargetKey.ResolveSelectedKey(*Asset.GetBlackboardAsset());
	ResultKey.ResolveSelectedKey(*Asset.GetBlackboardAsset());
}

EBTNodeResult::Type UBTTask_CheckFacingPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get self
	AActor* Self = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(SelfKey.SelectedKeyName));
	// Get the player
	AActor* Player = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName));

	FVector DirectionToPlayer;
	FVector ForwardDirection;
	if (Self) {
		ForwardDirection = Self->GetActorForwardVector();
		if (Player)
			DirectionToPlayer = FVector(Player->GetActorLocation().X - Self->GetActorLocation().X, 0.f, 0.f);
	}

	float Theta = FMath::Acos((FVector::DotProduct(DirectionToPlayer, ForwardDirection)) / (DirectionToPlayer.Size() * ForwardDirection.Size()));
	
	if (FMath::IsNearlyZero(Theta, 0.01f)) {
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(ResultKey.SelectedKeyName, true);
		return EBTNodeResult::Succeeded;
	}
	else {
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(ResultKey.SelectedKeyName, false);
		return EBTNodeResult::Failed;
	}
}
