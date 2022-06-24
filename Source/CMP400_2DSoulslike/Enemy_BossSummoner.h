// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CharacterBase.h"
#include "Enemy_BossSummoner.generated.h"

UCLASS()
class CMP400_2DSOULSLIKE_API AEnemy_BossSummoner : public ACharacterBase
{
	GENERATED_BODY()

protected:
	/* Gameplay Data */
	// Attack
	bool bAttacking;
	bool bSaveAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Attacks)
		float MeleeDamage;
	int32 AttackCount;
	struct FTimerHandle AttackTimer;
	class AAttackHitbox* MeleeHitbox;

	// Summon
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Dash)
		float SummonCooldown;
	float SummonTimer;
	bool bCanSummon;

	// Heal
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Dash)
		float HealPower;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Dash)
		float HealCooldown;
	float HealTimer;
	bool bCanHeal;

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
		UPaperFlipbook* HealAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* IdleAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* RunAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* SummonAnim;

	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* AltAttackAnim[2];
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* AltDeathAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* AltHealAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* AltIdleAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* AltRunAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* AltSummonAnim;

	// Audio
	UPROPERTY(EditAnywhere, Category = Wwise)
		UAkAudioEvent* AttackEvent[2];
	UPROPERTY(EditAnywhere, Category = Wwise)
		UAkAudioEvent* CastEvent;

public:
	// Sets default values for this character's properties
	AEnemy_BossSummoner();

protected:
	// Overrides
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
	virtual void Tick(float DeltaTime) override;

	/* Combat */
	// Attacks
	UFUNCTION()
		void Attack(int32 AttackIndex);
	void AttackTriggered();
	UFUNCTION()
		void DamageEnemies();
	void ComboAttack();
	void ResetCombo();

	// Summon Spell
	UFUNCTION()
		void SummonWisps(const AActor* Target);

	// Healing Spell
	UFUNCTION()
		void Heal();

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


	inline bool CanHeal() { return bCanHeal; }
	inline bool CanSummon() { return bCanSummon; }
};
