// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_KeepDistanceFrom.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "AIController.h"

UBTTask_KeepDistanceFrom::UBTTask_KeepDistanceFrom()
{
	Radius = 750.f;

	SelfKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_KeepDistanceFrom, SelfKey), AActor::StaticClass());
	TargetKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_KeepDistanceFrom, TargetKey));
	TargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_KeepDistanceFrom, TargetKey), AActor::StaticClass());
}

void UBTTask_KeepDistanceFrom::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	SelfKey.ResolveSelectedKey(*Asset.GetBlackboardAsset());
	TargetKey.ResolveSelectedKey(*Asset.GetBlackboardAsset());
}

EBTNodeResult::Type UBTTask_KeepDistanceFrom::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* SelfController = OwnerComp.GetAIOwner();
	if (!SelfController)
		return EBTNodeResult::Failed;

	AActor* Self = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(SelfKey.SelectedKeyName));
	FVector LeftTargetLoc, RightTargetLoc;

	if (TargetKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName));
		LeftTargetLoc = TargetActor->GetActorLocation() - FVector(Radius, 0.f, 0.f);
		LeftTargetLoc.Z = Self->GetActorLocation().Z;
		RightTargetLoc = TargetActor->GetActorLocation() + FVector(Radius, 0.f, 0.f);
		RightTargetLoc.Z = Self->GetActorLocation().Z;
	}
	else if (TargetKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		FVector TargetVector = OwnerComp.GetBlackboardComponent()->GetValueAsVector(TargetKey.SelectedKeyName);
		LeftTargetLoc = TargetVector - FVector(Radius, 0.f, 0.f);
		LeftTargetLoc.Z = Self->GetActorLocation().Z;
		RightTargetLoc = TargetVector + FVector(Radius, 0.f, 0.f);
		RightTargetLoc.Z = Self->GetActorLocation().Z;
	}

	if (FVector::Dist(Self->GetActorLocation(), LeftTargetLoc) < FVector::Dist(Self->GetActorLocation(), RightTargetLoc)) {
		if (FVector::Dist(Self->GetActorLocation(), LeftTargetLoc) > 50)
			SelfController->MoveToLocation(LeftTargetLoc, 50.f);

		return EBTNodeResult::Succeeded;
	}
	else if (FVector::Dist(Self->GetActorLocation(), LeftTargetLoc) > FVector::Dist(Self->GetActorLocation(), RightTargetLoc)) {
		if (FVector::Dist(Self->GetActorLocation(), RightTargetLoc) > 50.f)
			SelfController->MoveToLocation(RightTargetLoc, 50.f);

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
