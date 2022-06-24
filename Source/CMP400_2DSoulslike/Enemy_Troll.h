// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CharacterBase.h"
#include "Enemy_Troll.generated.h"

UCLASS()
class CMP400_2DSOULSLIKE_API AEnemy_Troll : public ACharacterBase
{
	GENERATED_BODY()

protected:
	/* Gameplay Data */
	// Attack
	bool bAttacking;
	bool bSaveAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Attacks)
		float SlamDamage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Attacks)
		float SlamCost;
	class AAttackHitbox* SlamHitbox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Attacks)
		float CastDamage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Attacks)
		float CastCost;

	// AI
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	//	class UBehaviorTree* BehaviorTree;

	// Animation 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animations, meta = (AllowPrivateAccess = "true"))
		EAnimState AnimState;

	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* AttackAnim[2];
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* DeathAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* IdleAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* RunAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* HurtAnim;

	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* AltAttackAnim[2];
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* AltDeathAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* AltIdleAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* AltRunAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* AltHurtAnim;

	// Audio
	UPROPERTY(EditAnywhere, Category = Wwise)
		UAkAudioEvent* AttackEvent;
	UPROPERTY(EditAnywhere, Category = Wwise)
		UAkAudioEvent* CastEvent;
	UPROPERTY(EditAnywhere, Category = Wwise)
		UAkAudioEvent* LandEvent;
	UPROPERTY(EditAnywhere, Category = Wwise)
		UAkAudioEvent* FootstepEvent;

	struct FTimerHandle FootTimer[2];


public:
	// Sets default values for this character's properties
	AEnemy_Troll();

protected:
	// Overrides
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
	virtual void Tick(float DeltaTime) override;
	virtual void Landed(const FHitResult& Hit) override;

	/* Combat */
	// Attacks
	UFUNCTION()
		void Slam(int32 AttackIndex);
	UFUNCTION()
		void DamageEnemies();
	UFUNCTION()
		void CastAttack(const AActor* Target);
	UFUNCTION()
		void SpawnSpell(AActor* Target);

	// anim
	//void PlayDeathFeedback();

	/* Anims */
	void UpdateCharacter();
	void UpdateAnimation();

	/*Audio*/
	UFUNCTION()
		void PlaySfx(UAkAudioEvent* SfxToPlay, bool bTriggerSfxOnCompass = false);

public:

	// Methods to be bound to event
	UFUNCTION()
		virtual void TakeHit(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser) override;
	
};
