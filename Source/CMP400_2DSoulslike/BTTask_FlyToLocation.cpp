// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_FlyToLocation.h"

#include "Kismet/GameplayStatics.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/KismetMathLibrary.h"

UBTTask_FlyToLocation::UBTTask_FlyToLocation()
{
	bNotifyTick = true;
	AcceptanceRadius = 50.f;
	bTrackMovingGoal = false;
	bUseLerp = true;
	FlightSpeed = 400.f;

	SelfKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FlyToLocation, SelfKey), AActor::StaticClass());
	TargetKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FlyToLocation, TargetKey));
	TargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FlyToLocation, TargetKey), AActor::StaticClass());
}

void UBTTask_FlyToLocation::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	SelfKey.ResolveSelectedKey(*Asset.GetBlackboardAsset());
	TargetKey.ResolveSelectedKey(*Asset.GetBlackboardAsset());
}

EBTNodeResult::Type UBTTask_FlyToLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	Self = Cast<ACharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(SelfKey.SelectedKeyName));

	if (TargetKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName));
		TargetLoc = TargetActor->GetActorLocation();
	}
	else if (TargetKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		FVector TargetVector = OwnerComp.GetBlackboardComponent()->GetValueAsVector(TargetKey.SelectedKeyName);
		TargetLoc = TargetVector;
	}

	InitLoc = Self->GetActorLocation();
	InitDist = FVector::Distance(InitLoc, TargetLoc);

	return EBTNodeResult::InProgress;
}

void UBTTask_FlyToLocation::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	float DistToTarget;
	if (Self) {
		DistToTarget = FVector::Distance(Self->GetActorLocation(), TargetLoc);
	}
	else {
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	if (bTrackMovingGoal) {
		if (TargetKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass()) {
			AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName));
			TargetLoc = TargetActor->GetActorLocation();
		}
		else if (TargetKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass()) {
			FVector TargetVector = OwnerComp.GetBlackboardComponent()->GetValueAsVector(TargetKey.SelectedKeyName);
			TargetLoc = TargetVector;
		}
		InitDist = FVector::Distance(InitLoc, TargetLoc);
	}

	FVector Velocity;
	if (Self) {
		if (bUseLerp) {
			float Alpha = (InitDist - DistToTarget) / InitDist;
			Velocity = UKismetMathLibrary::VLerp(UKismetMathLibrary::GetDirectionUnitVector(Self->GetActorLocation(), TargetLoc), FVector(0.f), Alpha) * FlightSpeed;
		}
		else {
			Velocity = UKismetMathLibrary::GetDirectionUnitVector(Self->GetActorLocation(), TargetLoc) * FlightSpeed;
		}
	}
	else {
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	if (Self) {
		if (Self->GetCharacterMovement()->Velocity.Size() <= Self->GetCharacterMovement()->MaxFlySpeed)
			Self->GetCharacterMovement()->Velocity = Velocity;
	}
	else {
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	if (Self) {
		if (DistToTarget <= AcceptanceRadius) {

			if (!bUseLerp)
				Self->GetCharacterMovement()->Velocity *= 0.1f;
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
	else {
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}
