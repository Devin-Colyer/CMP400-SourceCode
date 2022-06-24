// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_FindFlyingLocationInRange.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "PassThroughPlatform.h"

FVector UBTTask_FindFlyingLocationInRange::GetNewLocation(AActor* Self)
{
	FVector RandDir = UKismetMathLibrary::RandomUnitVector();
	float Dist = UKismetMathLibrary::RandomFloatInRange(MinDistance, Radius);
	FVector Target = Self->GetActorLocation() + (RandDir * Dist);
	Target.Y = 10.f;
	
	return Target;
}

UBTTask_FindFlyingLocationInRange::UBTTask_FindFlyingLocationInRange()
{
	MinDistance = 100.f;
	Radius = 250.f;

	SelfKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FindFlyingLocationInRange, SelfKey), AActor::StaticClass());
	ResultKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FindFlyingLocationInRange, ResultKey));
}

void UBTTask_FindFlyingLocationInRange::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	SelfKey.ResolveSelectedKey(*Asset.GetBlackboardAsset());
	ResultKey.ResolveSelectedKey(*Asset.GetBlackboardAsset());
}

EBTNodeResult::Type UBTTask_FindFlyingLocationInRange::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AActor* Self = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(SelfKey.SelectedKeyName));

	TArray<AActor*> ActorsToIgnore;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APassThroughPlatform::StaticClass(), ActorsToIgnore);
	ActorsToIgnore.Add((AActor*)UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	ActorsToIgnore.Add(Self);

	FVector Origin, BoxExtent;
	Self->GetActorBounds(true, Origin, BoxExtent);
	BoxExtent.Y = 1.f;

	FVector Target;
	for (size_t i = 0; i < 25; i++)
	{
		Target = GetNewLocation(Self);
		FHitResult HitOut;
		if (!UKismetSystemLibrary::BoxTraceSingle(GetWorld(), Self->GetActorLocation(), Target, BoxExtent, FRotator(),
			UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_WorldStatic), false, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, HitOut, true))
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(ResultKey.SelectedKeyName, Target);
			return EBTNodeResult::Succeeded;
		}
	}
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(ResultKey.SelectedKeyName, Self->GetActorLocation());
	return EBTNodeResult::Failed;
}
