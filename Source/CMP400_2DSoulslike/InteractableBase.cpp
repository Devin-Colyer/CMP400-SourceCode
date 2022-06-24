// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableBase.h"

#include "Components/SphereComponent.h"

#include "Blueprint/UserWidget.h"

#include "Kismet/GameplayStatics.h"

#include "PlayerCharacter.h"
#include "SoulslikePlayerController.h"
#include "PlayerHUD.h"
#include "InteractableWidget.h"

// Sets default values
AInteractableBase::AInteractableBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Range = CreateDefaultSubobject<USphereComponent>("Hitbox");
	Range->SetupAttachment(RootComponent);
	Range->SetSphereRadius(100.f);
	Range->SetRelativeLocation(FVector(0.f));
	Range->SetGenerateOverlapEvents(true);
	Range->SetCollisionResponseToChannels(ECollisionResponse::ECR_Overlap);

	bInteracting = false;
}

void AInteractableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<AActor*>Actors;
	Range->GetOverlappingActors(Actors, APlayerCharacter::StaticClass());
	for (AActor* Actor : Actors) {

		APlayerCharacter* Player = Cast<APlayerCharacter>(Actor);
		if (!Player)
			break;

		ASoulslikePlayerController* Ctrl = Player->GetPlayerController();
		if (!Ctrl)
			break;

		if (!Ctrl->IsMenuOpen()) {
			AInteractableBase* PlayerCurrentInteractable = Player->GetInteractableObject();
			if (PlayerCurrentInteractable && PlayerCurrentInteractable != this && !bInteracting) {
				double MyDist = FVector::DistSquared(GetActorLocation(), Player->GetActorLocation());
				double OtherDist = FVector::DistSquared(PlayerCurrentInteractable->GetActorLocation(), Player->GetActorLocation());
				if (MyDist > OtherDist) {
					AddPrompt();
					Player->SetInteractable(this);
				}
			}
			else if (PlayerCurrentInteractable == nullptr) {
				AddPrompt();
				Player->SetInteractable(this);
			}
		}
		else {
			RemovePrompt(Actor);
		}
	}
}

void AInteractableBase::AddPrompt()
{
	if (PromptWidgetClass) {
		PromptWidget = CreateWidget<UInteractableWidget>(GetWorld(), PromptWidgetClass);

		if (PromptWidget)
			PromptWidget->AddToViewport();

		UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD<APlayerHUD>()->ReorderDecal();
	}
}

void AInteractableBase::RemovePrompt(AActor* OtherActor)
{
	APlayerCharacter* Actor = Cast<APlayerCharacter>(OtherActor);
	if (Actor) {
		if (PromptWidget)
			PromptWidget->RemoveFromParent();
		PromptWidget = nullptr;
		Actor->SetInteractable(nullptr);
	}
}