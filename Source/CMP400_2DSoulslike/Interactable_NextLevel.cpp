// Fill out your copyright notice in the Description page of Project Settings.

//PRAGMA_DISABLE_OPTIMIZATION;

#include "Interactable_NextLevel.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"

#include "Components/SphereComponent.h"

#include "InteractableWidget.h"
#include "PlayerCharacter.h"

#include "WriteToFileManager.h"

AInteractable_NextLevel::AInteractable_NextLevel()
{
	OnInteract.AddDynamic(this, &AInteractable_NextLevel::NextLevel);
	Range->OnComponentEndOverlap.AddDynamic(this, &AInteractable_NextLevel::OnEndOverlap);

	TimeToComplete = 0.f;
}

void AInteractable_NextLevel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeToComplete += DeltaTime;
	
	if (PromptWidget)
		PromptWidget->ActionText = FText::FromString("progress to the next area");
}

void AInteractable_NextLevel::NextLevel()
{
	FString Msg = FDateTime::Now().ToString() + ',' + UGameplayStatics::GetCurrentLevelName(GetWorld()) + ',' + UKismetStringLibrary::TimeSecondsToString(TimeToComplete);
	//WriteToFileManager::AppendLineToFile(Msg, UKismetSystemLibrary::GetProjectSavedDirectory(), "TimeToLevelComplete.csv");

	//UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), Level, true, Options.ToString());

	FString LevelName = Level.GetAssetName();
	LevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	UGameplayStatics::OpenLevel(GetWorld(), FName(LevelName), true, Options.ToString());
}

void AInteractable_NextLevel::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerCharacter* Actor = Cast<APlayerCharacter>(OtherActor);
	if (Actor) {
		if (PromptWidget)
			PromptWidget->RemoveFromParent();
		PromptWidget = nullptr;
		bInteracting = false;
	}
	RemovePrompt(OtherActor);
}
