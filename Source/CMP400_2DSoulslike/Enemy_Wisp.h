// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CharacterBase.h"
#include "Enemy_Wisp.generated.h"

UCLASS()
class CMP400_2DSOULSLIKE_API AEnemy_Wisp : public ACharacterBase
{
	GENERATED_BODY()

protected:
	/* Gameplay Data */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Attacks)
		float MeleeDamage;

	// AI
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	//	class UBehaviorTree* BehaviorTree;

	// Animation
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animations, meta = (AllowPrivateAccess = "true"))
		EAnimState AnimState;

	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* DeathAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* RunAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* SummonAnim;

	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* AltDeathAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* AltRunAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* AltSummonAnim;

public:
	// Sets default values for this character's properties
	AEnemy_Wisp();

protected:
	// Overrides
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
	virtual void Tick(float DeltaTime) override;

	/* Lifetime */
	void KillSelf();

	/* Anims */
	void UpdateCharacter();
	void UpdateAnimation();

	/*Audio*/
	UFUNCTION()
		void PlaySfx(UAkAudioEvent* SfxToPlay, bool bTriggerSfxOnCompass = false);

public:

	// Methods to be bound to event
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		virtual void TakeHit(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser) override;
	
};
