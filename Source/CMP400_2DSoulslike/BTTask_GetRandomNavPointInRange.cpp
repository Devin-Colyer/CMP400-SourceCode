// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetRandomNavPointInRange.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UBTTask_GetRandomNavPointInRange::UBTTask_GetRandomNavPointInRange()
{
	Radius = 250.f;

	SelfKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_GetRandomNavPointInRange, SelfKey), AActor::StaticClass());
	ResultKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_GetRandomNavPointInRange, ResultKey));
}

void UBTTask_GetRandomNavPointInRange::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	SelfKey.ResolveSelectedKey(*Asset.GetBlackboardAsset());
	ResultKey.ResolveSelectedKey(*Asset.GetBlackboardAsset());
}

EBTNodeResult::Type UBTTask_GetRandomNavPointInRange::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get the controlled pawn from the blackboard
	AActor* Self = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(SelfKey.SelectedKeyName));

	FNavLocation NavLocation;

	// Get access to the Unreal Nav System
	const UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());

	//Get a random location on the nav mesh, within a radius
	NavSys->GetRandomReachablePointInRadius(Self->GetActorLocation(), Radius, NavLocation);

	// Set AI's target location to the new location
	FVector Location2D = FVector(NavLocation.Location.X, 0.f, NavLocation.Location.Z);
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(ResultKey.SelectedKeyName, Location2D);
	return EBTNodeResult::Succeeded;
}
