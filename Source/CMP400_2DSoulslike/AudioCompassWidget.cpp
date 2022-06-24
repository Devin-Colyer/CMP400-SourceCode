// Fill out your copyright notice in the Description page of Project Settings.


#include "AudioCompassWidget.h"

// Kismet
#include "Kismet/GameplayStatics.h"

// Framework
#include "GameFramework/Pawn.h"
#include "Misc/ScopeLock.h"

// UI
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetTree.h"

// Timer
#include "TimerManager.h"

UAudioCompassWidget::UAudioCompassWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UWidget>CompassArrowAsset(TEXT("WidgetBlueprint'/Game/UI/HUD/PointerWidget.PointerWidget_C'"));
	ArrowWidgetClass = CompassArrowAsset.Class;
	OnSfxTriggered.AddDynamic(this, &UAudioCompassWidget::SfxTriggered);
}

void UAudioCompassWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	TimerManager.Tick(InDeltaTime);

	// Update all arrows
	if (CompassMutex.try_lock()) {

		FVector PlayerLoc3D = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation();
		FVector PlayerUpVector = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorUpVector();

		for (size_t i = 0; i < CompassArrows.Num(); i++)
		{
			FVector2D PlayerToSource = CompassArrows[i]->SfxSource - FVector2D(PlayerLoc3D.X, PlayerLoc3D.Z);
			float Theta = FMath::RadiansToDegrees(FMath::Acos(FVector2D::DotProduct(FVector2D(PlayerUpVector.X, PlayerUpVector.Z), PlayerToSource) / (PlayerUpVector.Size() * PlayerToSource.Size())));
			
			if (!CompassArrows[i]) {
				CompassArrows.RemoveAt(i);
				CompassMutex.unlock();
				return;
			}

			if (!CompassArrows[i]->Arrow) {
				CompassArrows.RemoveAt(i);
				CompassMutex.unlock();
				return;
			}

			if (CompassArrows[i]->SfxSource.X >= PlayerLoc3D.X)
				CompassArrows[i]->Arrow->SetRenderTransformAngle(Theta);
			if (CompassArrows[i]->SfxSource.X < PlayerLoc3D.X)
				CompassArrows[i]->Arrow->SetRenderTransformAngle(-Theta);
		}
		CompassMutex.unlock();
	}
}

void UAudioCompassWidget::SfxTriggered(FVector SfxLocation, float SfxDuration)
{
	UPanelWidget* RootWidget = Cast<UPanelWidget>(GetRootWidget());
	int32 NextIndex = CompassArrows.Num();

	// Make new img
	UWidget* NewImage = WidgetTree->ConstructWidget<UWidget>(ArrowWidgetClass, FName("Arrow_" + FString::FromInt(NextIndex)));
	RootWidget->AddChild(NewImage);

	UCanvasPanelSlot* ImageCanvasPanelSlot = Cast<UCanvasPanelSlot>(NewImage->Slot);
	ImageCanvasPanelSlot->SetAnchors(FAnchors(0.5f, 0.5f, 0.5f, 0.5f));
	ImageCanvasPanelSlot->SetSize(FVector2D(848.f, 4000.f));
	ImageCanvasPanelSlot->SetAlignment(FVector2D(0.5f, 0.5f));
	ImageCanvasPanelSlot->SetPosition(FVector2D(0.f, 50.f));

	NewImage->SetRenderScale(FVector2D(0.1f, 0.1f));

	// Track new img
	CompassArrows.Add(NewObject<UCompassArrow>());
	CompassArrows[NextIndex]->Arrow = NewImage;
	CompassArrows[NextIndex]->SfxSource = FVector2D(SfxLocation.X, SfxLocation.Z);

	// Set timer
	FTimerHandle SfxTimer;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, "RemoveArrow", CompassArrows[NextIndex], SfxTimer);
	TimerManager.SetTimer(SfxTimer, TimerDelegate, SfxDuration, false);
}

void UAudioCompassWidget::RemoveArrow(UCompassArrow* Arrow, FTimerHandle AssociatedTimer)
{
	// Try delete arrow
	if (CompassMutex.try_lock()) {

		CompassArrows.Remove(Arrow);
		Arrow->Arrow->RemoveFromParent();
		//WidgetTree->RemoveWidget(Arrow.Arrow);
		//Arrow.Arrow->RemoveFromRoot();

		CompassMutex.unlock();
	}
	else {
		// Try again
		FTimerHandle SfxTimer;
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, "RemoveArrow", Arrow, SfxTimer);
		TimerManager.SetTimer(SfxTimer, TimerDelegate, 0.1f, false);
	}
	TimerManager.ClearTimer(AssociatedTimer);
}
