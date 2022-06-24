// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactable_Campsite.h"

// Kismet
#include "Kismet/GameplayStatics.h"

// Comps
#include "Components/SphereComponent.h"

// Misc
#include "Misc/DateTime.h"

// My headers
#include "InteractableWidget.h"
#include "AdventureGameState.h"
#include "PlayerCharacter.h"
#include "EnemySpawner.h"

#include "WriteToFileManager.h"

AInteractable_Campsite::AInteractable_Campsite()
{
	OnInteract.AddDynamic(this, &AInteractable_Campsite::RestAtCamp);
	Range->OnComponentEndOverlap.AddDynamic(this, &AInteractable_Campsite::OnEndOverlap);
}

void AInteractable_Campsite::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PromptWidget)
		PromptWidget->ActionText = FText::FromString("rest at campsite");
}

void AInteractable_Campsite::RestAtCamp()
{
	AAdventureGameState* GameState = Cast<AAdventureGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (GameState) {
		// SetSpawnPoint
		GameState->SetRespawnTransform(GetActorTransform());

		// Heal player
		APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (Player) {
			FString Msg = FDateTime::Now().ToString() + ',' + FString::SanitizeFloat(Player->GetHealthVal());
			//WriteToFileManager::AppendLineToFile(Msg, UKismetSystemLibrary::GetProjectSavedDirectory(), "RestHealth.csv");
			Player->UpdateHealth(BIG_NUMBER);
			Player->UpdateStamina(BIG_NUMBER);
		}

		// reset enemies
		TArray<AActor*>Actors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacterBase::StaticClass(), Actors);
		for (AActor* Enemy : Actors) {
			if (!Cast<APlayerCharacter>(Enemy))
				Enemy->Destroy();
		}

		Actors.Empty();
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemySpawner::StaticClass(), Actors);
		for (AActor* Spawner : Actors) {
			Cast<AEnemySpawner>(Spawner)->Spawn();
		}
	}
}

void AInteractable_Campsite::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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
