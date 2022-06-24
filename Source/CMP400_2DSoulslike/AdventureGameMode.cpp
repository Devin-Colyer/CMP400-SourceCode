// Fill out your copyright notice in the Description page of Project Settings.


#include "AdventureGameMode.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// My Headers
#include "PlayerCharacter.h"
#include "PlayerHUD.h"
#include "SoulslikePlayerController.h"
#include "AdventureGameState.h"

#include "WriteToFileManager.h"

AAdventureGameMode::AAdventureGameMode()
{
	// Set default pawn class to our character
	DefaultPawnClass = APlayerCharacter::StaticClass();
	HUDClass = APlayerHUD::StaticClass();
	PlayerControllerClass = ASoulslikePlayerController::StaticClass();
	GameStateClass = AAdventureGameState::StaticClass();
}

void AAdventureGameMode::BeginPlay()
{
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	//WriteToFileManager::WriteLineToFile("Timestamp,Health", UKismetSystemLibrary::GetProjectSavedDirectory(), "RestHealth.csv", false);
	//WriteToFileManager::WriteLineToFile("Timestamp,Level Name,Time", UKismetSystemLibrary::GetProjectSavedDirectory(), "TimeToDeath.csv", false);
	//WriteToFileManager::WriteLineToFile("Timestamp,Level Name,Time", UKismetSystemLibrary::GetProjectSavedDirectory(), "TimeToLevelComplete.csv", false);
	//WriteToFileManager::WriteLineToFile("Timestamp,Level Name,Last Damage Causer,Pos 1,Pos 2,Pos 3,Pos 4,Pos 5,Death Pos", UKismetSystemLibrary::GetProjectSavedDirectory(), "DeathPositionQueue.csv", false);
}
