// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController_Wisp.h"

// AI
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"

// Perception
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"

// Kismet
#include "Kismet/GameplayStatics.h"

//My headers
#include "Enemy_Wisp.h"
#include "PlayerCharacter.h"

AEnemyController_Wisp::AEnemyController_Wisp(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bSetControlRotationFromPawnOrientation = false;

	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component"));
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard Component"));

	BehaviorTree = Cast<UBehaviorTree>(StaticLoadObject(UBehaviorTree::StaticClass(), nullptr, TEXT("BehaviorTree'/Game/AI/Boss/Wisp_BT.Wisp_BT'")));

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SightConfig->SightRadius = 2000.f;
	SightConfig->LoseSightRadius = 5000.f;
	SightConfig->PeripheralVisionAngleDegrees = 360.f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	AIPerceptionComp->ConfigureSense(*SightConfig);
	AIPerceptionComp->SetSenseEnabled(SightConfig->GetSenseImplementation(), true);

	//HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
	//HearingConfig->HearingRange = 3000.f;
	//HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	//HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
	//HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
	//AIPerceptionComp->ConfigureSense(*HearingConfig);
	//AIPerceptionComp->SetSenseEnabled(HearingConfig->GetSenseImplementation(), true);

	AIPerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyController_Wisp::OnTargetPerceptionUpdated);

	/*GetActionsComp();
	GetPathFollowingComponent();*/
}

void AEnemyController_Wisp::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AEnemy_Wisp* Self = Cast<AEnemy_Wisp>(InPawn);
	if (Self) {
		if (BehaviorTree->BlackboardAsset)
			Blackboard->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
		RunBehaviorTree(BehaviorTree);

		Blackboard->SetValueAsObject("SelfActor", Self);
		Blackboard->SetValueAsBool("bDeathSequenceStarted", false);
	}
}

void AEnemyController_Wisp::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	TArray<AActor*> SeenActors, HeardActors;
	AIPerceptionComp->GetCurrentlyPerceivedActors(TSubclassOf<UAISense_Sight>(), SeenActors);
	AIPerceptionComp->GetCurrentlyPerceivedActors(TSubclassOf<UAISense_Hearing>(), HeardActors);

	/*	if (Stimulus.Tag == FName("Sound")) {
			if (HeardActors.Contains(Actor)) {
				Blackboard->SetValueAsVector("TargetLocation", Stimulus.StimulusLocation);
			}
		}
		else*/ if (SeenActors.Contains(Actor)) {
			if (Cast<APlayerCharacter>(Actor)) {
				Blackboard->SetValueAsObject("DetectedCharacter", Actor);
			}
		}
}



