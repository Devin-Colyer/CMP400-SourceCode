// Fill out your copyright notice in the Description page of Project Settings.


#include "AdventureGameState.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"

#include "PlayerCharacter.h"
#include "SoulslikePlayerController.h"
#include "PlayerHUD.h"
#include "EnemySpawner.h"

#include "WriteToFileManager.h"

AAdventureGameState::AAdventureGameState()
{
	// Set this game mode to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<UUserWidget>GameOverAsset(TEXT("WidgetBlueprint'/Game/UI/HUD/YouDiedWidget.YouDiedWidget_C'"));
	GameOverWidgetClass = GameOverAsset.Class;

	//OnEnemyKilled.AddDynamic(this, &AAdventureGameState::EnemyKilled);

	// defaults
	TimeToDeath = 0.f;
	Kills = 0;
	DeathTimer = 0;
	ScoreSaveName = "HighScoreSlot";
}

void AAdventureGameState::BeginPlay()
{
	Super::BeginPlay();

	LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());

	SetCurrentState(EGameplayState::EGS_Playing);
	// get typecast pointer to player character
	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	PlayerCharacter->OnDestroyed.AddDynamic(this, &AAdventureGameState::PlayerKilled);

	RespawnTransform = PlayerCharacter->GetTransform();
}

void AAdventureGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	TimeToDeath += DeltaTime;

	switch (CurrentState) {
	case EGameplayState::EGS_Playing:
		TickPlaying(DeltaTime);
		break;
	case EGameplayState::EGS_GameOver:
		TickGameOver(DeltaTime);
		break;
	default:
		UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, true);
		break;
	}
}

void AAdventureGameState::EnemyKilled()
{
	Kills++;
}

void AAdventureGameState::PlayerKilled(AActor* DestroyedActor)
{
	FString Msg = FDateTime::Now().ToString() + ',' + LevelName + ',' + UKismetStringLibrary::TimeSecondsToString(TimeToDeath);
	//WriteToFileManager::AppendLineToFile(Msg, UKismetSystemLibrary::GetProjectSavedDirectory(), "TimeToDeath.csv");
	TimeToDeath = 0.f;

	ASoulslikePlayerController* Ctrl = Cast<APlayerCharacter>(DestroyedActor)->GetPlayerController();
	Ctrl->UnPossess();

	PlayerCharacter = nullptr;
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	PlayerCharacter = GetWorld()->SpawnActor<APlayerCharacter>(RespawnTransform.GetLocation(), RespawnTransform.GetRotation().Rotator(), SpawnParams);
	
	if (PlayerCharacter) {
		PlayerCharacter->OnDestroyed.AddDynamic(this, &AAdventureGameState::PlayerKilled);
		Ctrl->Possess(PlayerCharacter);
		//UGameplayStatics::GetGameMode(GetWorld())->RestartPlayer(Cast<APlayerCharacter>(DestroyedActor)->GetPlayerController());
		//UGameplayStatics::GetPlayerController(GetWorld(), 0)->Possess(PlayerCharacter);
	}
}

void AAdventureGameState::HandleNewState(const EGameplayState InNewState)
{
	switch (InNewState) {
	case EGameplayState::EGS_Playing:
		HandlePlaying();
		break;
	case EGameplayState::EGS_GameOver:
		HandleGameOver();
		break;
	default:
		UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, true);
		break;
	}
}

void AAdventureGameState::HandlePlaying()
{
}

void AAdventureGameState::HandleGameOver()
{
	TArray<AActor*>Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacterBase::StaticClass(), Actors);
	for (AActor* Enemy : Actors) {
		if (!Cast<APlayerCharacter>(Enemy))
			Enemy->Destroy();
	}

	/*UScoreSaveGame* SaveToUpdate;
	UGameplayStatics::DoesSaveGameExist(ScoreSaveName, 0) ?
		SaveToUpdate = Cast<UScoreSaveGame>(UGameplayStatics::LoadGameFromSlot(ScoreSaveName, 0)) :
		SaveToUpdate = Cast<UScoreSaveGame>(UGameplayStatics::CreateSaveGameObject(UScoreSaveGame::StaticClass()));

	SaveToUpdate->InsertNewScore(Time, Score, Kills);
	UGameplayStatics::SaveGameToSlot(SaveToUpdate, ScoreSaveName, 0);*/

	if (GameOverWidgetClass) {
		GameOverWidget = CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass);

		if (GameOverWidget)
			GameOverWidget->AddToViewport();
	}

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD<APlayerHUD>()->ReorderDecal();
}

void AAdventureGameState::TickPlaying(float DeltaTime)
{
	// if player character exists, check the health percentage, if less than .01% it's game over
	if (PlayerCharacter)
		if (FMath::IsNearlyZero(PlayerCharacter->GetHealth(), 0.0001f))
			SetCurrentState(EGameplayState::EGS_GameOver);
}

void AAdventureGameState::TickGameOver(float DeltaTime)
{
	if (DeathTimer >= 2.5f) {
		if (GameOverWidget)
			GameOverWidget->RemoveFromParent();

		PlayerCharacter->Destroy();

		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemySpawner::StaticClass(), Actors);
		for (AActor* Spawner : Actors) {
			Cast<AEnemySpawner>(Spawner)->Spawn();
		}

		DeathTimer = 0;
		SetCurrentState(EGameplayState::EGS_Playing);
	}

	DeathTimer += DeltaTime;
}

void AAdventureGameState::SetCurrentState(const EGameplayState InNewState)
{
	CurrentState = InNewState;
	HandleNewState(CurrentState);
}

void AAdventureGameState::SetRespawnTransform(FTransform InTransform)
{
	RespawnTransform = InTransform;
}


