// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable_Dialogue.h"

#include "Components/SphereComponent.h"

#include "Kismet/GameplayStatics.h"

#include "DialogueWidget.h"
#include "InteractableWidget.h"
#include "SoulslikePlayerController.h"
#include "PlayerHUD.h"
#include "PlayerCharacter.h"

AInteractable_Dialogue::AInteractable_Dialogue()
{
	OnInteract.AddDynamic(this, &AInteractable_Dialogue::NextLine);
	Range->OnComponentEndOverlap.AddDynamic(this, &AInteractable_Dialogue::OnEndOverlap);
}

void AInteractable_Dialogue::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DialogueWidget) {
		DialogueWidget->PromptTemplate = FText::FromString("{Key} for {Action}");
		DialogueWidget->ActionText = FText::FromString("next line");
		if (!DialogueWidget->IsInViewport()) {
			DialogueWidget = nullptr;
			bInteracting = false;
			Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->SetInteractable(nullptr);
		}
	}

	if (PromptWidget)
		PromptWidget->ActionText = FText::FromString("read");
}

void AInteractable_Dialogue::NextLine()
{
	if (!DialogueWidget)
		if (DialogueWidgetClass) {
			DialogueWidget = CreateWidget<UDialogueWidget>(GetWorld(), DialogueWidgetClass);

			if (DialogueWidget) {
				DialogueWidget->AddToViewport();
				bInteracting = true;
			}

			UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD<APlayerHUD>()->ReorderDecal();
		}

	if (PromptWidget) {
		PromptWidget->RemoveFromParent();
		PromptWidget = nullptr;
	}

	DialogueWidget->NextLine();
}

void AInteractable_Dialogue::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerCharacter* Actor = Cast<APlayerCharacter>(OtherActor);
	if (Actor) {
		if (DialogueWidget)
			DialogueWidget->RemoveFromParent();
		DialogueWidget = nullptr;
		bInteracting = false;
	}
	RemovePrompt(OtherActor);
}
