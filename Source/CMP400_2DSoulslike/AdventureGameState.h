// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameStateBase.h"
#include "AdventureGameState.generated.h"


// Event dispatchers
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyKilled);

// Gameplay state enum
UENUM()
enum class EGameplayState : uint8 {
	EGS_Playing UMETA(DisplayName = "EGS_Playing"),
	EGS_GameOver UMETA(DisplayName = "EGS_GameOver"),
	EGS_Unknown UMETA(DisplayName = "EGS_Unknown")
};

UCLASS()
class CMP400_2DSOULSLIKE_API AAdventureGameState : public AGameStateBase
{
	GENERATED_BODY()

protected:
	// Pointer to player character
	class APlayerCharacter* PlayerCharacter;

	// Game state machine
	EGameplayState CurrentState;

	//Checkpoints
	FTransform RespawnTransform;

	// bragging rights
	FString LevelName;
	float TimeToDeath;
	int32 Kills;

	float DeathTimer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = SaveGame)
		FString ScoreSaveName;

	class USaveGame* HighScoreSave;

public:
	AAdventureGameState();

	UPROPERTY(EditAnywhere, Category = MainMenu)
		TSubclassOf<class UUserWidget> GameOverWidgetClass;
	UPROPERTY(EditAnywhere, Category = MainMenu)
		class UUserWidget* GameOverWidget;

	UPROPERTY(BlueprintCallable, Category = EventDispatchers)
		FOnEnemyKilled OnEnemyKilled;

protected:
	// Overridden methods
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void EnemyKilled();

	UFUNCTION()
		void PlayerKilled(AActor* DestroyedActor);

	// Called when new state is set
	void HandleNewState(const EGameplayState InNewState);

	void HandlePlaying();
	void HandleGameOver();

	void TickPlaying(float DeltaTime);
	void TickGameOver(float DeltaTime);

public:
	// Getters & setters
	UFUNCTION(BlueprintPure, Category = GameState)
		FORCEINLINE EGameplayState GetCurrentState() const { return CurrentState; }

	UFUNCTION(BlueprintCallable, Category = GameState)
		void SetCurrentState(const EGameplayState InNewState);

	UFUNCTION(BlueprintCallable, Category = Respawn)
		void SetRespawnTransform(FTransform InTransform);
	
};
