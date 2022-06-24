// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TrollSpellEffect.generated.h"

UCLASS()
class CMP400_2DSOULSLIKE_API ATrollSpellEffect : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animations, meta = (AllowPrivateAccess = "true"))
		class UPaperFlipbookComponent* FlipbookComponent;

	// Anim
	UPROPERTY(EditAnywhere, Category = Animations)
		class UPaperFlipbook* SpellAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		class UPaperFlipbook* AltSpellAnim;

	// Audio
	UPROPERTY(EditAnywhere, Category = Wwise)
		class UAkComponent* AudioComp;
	class FAkAudioDevice* AudioDevice;

	UPROPERTY(EditAnywhere, Category = Wwise)
		class UAkAudioEvent* StrikeEvent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		class UBoxComponent* Hitbox;

	float SpellDamage;
	bool bIsStrikeOn;

public:
	ATrollSpellEffect();

protected:
	void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
	void Tick(float DeltaTime) override;

	UFUNCTION()
		void TurnOnStrike();
	UFUNCTION()
		void TurnOffStrike();
	UFUNCTION()
		void Kill();

public:
	void InitializeSpell(float InSpellDamage, bool UseAltAnims);
};
