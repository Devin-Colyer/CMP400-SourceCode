// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyController_Troll.generated.h"

UCLASS()
class CMP400_2DSOULSLIKE_API AEnemyController_Troll : public AAIController
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class ACharacterBase* MyPawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behavior)
		class UAIPerceptionComponent* AIPerceptionComp;

	// AI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = Behavior)
		class UAISenseConfig_Sight* SightConfig;
	//UPROPERTY(EditDefaultsOnly, Category = Behavior)
	//	class UAISenseConfig_Hearing* HearingConfig;

public:
	AEnemyController_Troll(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
		void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};
