// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CharacterBase.h"
#include "Enemy_EvilWizard.generated.h"

UCLASS()
class CMP400_2DSOULSLIKE_API AEnemy_EvilWizard : public ACharacterBase
{
	GENERATED_BODY()

protected:
	/* Gameplay Data */
	// Attack
	bool bAttacking;
	bool bSaveAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Attacks)
		float FlameDamage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Attacks)
		float FlameCost;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Attacks)
		float FlameTickFrequency;

	bool bCanApplyDamage;
	struct FTimerHandle DamageTimer;

	class UBoxComponent* FlameHitbox;
	AActor* PlayerCharacter;

	// AI
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	//	class UBehaviorTree* BehaviorTree;

	// Animation 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animations, meta = (AllowPrivateAccess = "true"))
		EAnimState AnimState;

	UPROPERTY(EditAnywhere, Category = Animations)
		class UPaperFlipbook* AttackAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		class UPaperFlipbook* DeathAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		class UPaperFlipbook* IdleAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		class UPaperFlipbook* RunAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		class UPaperFlipbook* HurtAnim;

	UPROPERTY(EditAnywhere, Category = Animations)
		class UPaperFlipbook* AltAttackAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		class UPaperFlipbook* AltDeathAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		class UPaperFlipbook* AltIdleAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		class UPaperFlipbook* AltRunAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		class UPaperFlipbook* AltHurtAnim;

	// Audio
	UPROPERTY(EditAnywhere, Category = Wwise)
		UAkAudioEvent* StartAttackEvent;
	UPROPERTY(EditAnywhere, Category = Wwise)
		UAkAudioEvent* StopAttackEvent;


public:
	// Sets default values for this character's properties
	AEnemy_EvilWizard();

protected:
	// Overrides
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
	virtual void Tick(float DeltaTime) override;

	/* Combat */
	// Attacks
	UFUNCTION()
		void StartAttack();
	UFUNCTION()
		void StopAttack();
	UFUNCTION()
		void ApplyDamage();

	// anim
	//void PlayDeathFeedback();

	/* Anims */
	void UpdateCharacter();
	void UpdateAnimation();

	/*Audio*/
	UFUNCTION()
		void PlaySfx(UAkAudioEvent* SfxToPlay);

public:

	// Methods to be bound to event
	UFUNCTION()
		virtual void TakeHit(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser) override;
	// functions to be bound to events
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


};
