// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CharacterBase.h"
#include "Enemy_Archer.generated.h"

UCLASS()
class CMP400_2DSOULSLIKE_API AEnemy_Archer : public ACharacterBase
{
	GENERATED_BODY()


protected:
	/* Gameplay Data */
	// Attack
	bool bAttacking;
	bool bSaveAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Attacks)
		float ArrowDamage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Attacks)
		float ArrowSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Attacks)
		float ShootCost;

	// Dash
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		class UBoxComponent* DodgeTrigger;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Dash)
		float DashCost;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Dash)
		float DashPower;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Dash)
		float DashCooldown;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Dash)
		bool bDashing;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Dash)
		struct FTimerHandle DashTimer;

	// IFrames
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = IFrames)
		bool bCanTakeDamage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = IFrames)
		struct FTimerHandle IFrameTimer;

	// AI
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	//	class UBehaviorTree* BehaviorTree;

	// Animation 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animations, meta = (AllowPrivateAccess = "true"))
		EAnimState AnimState;

	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* AttackAnim[3];
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* DashAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* DeathAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* IdleAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* RunAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* HurtAnim;

	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* AltAttackAnim[3];
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* AltDashAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* AltDeathAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* AltIdleAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* AltRunAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* AltHurtAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UPaperSprite* ArrowSprite;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UPaperSprite* AltArrowSprite;

	// Audio
	UPROPERTY(EditAnywhere, Category = Wwise)
		UAkAudioEvent* AttackEvent;
	UPROPERTY(EditAnywhere, Category = Wwise)
		UAkAudioEvent* DashEvent;
	UPROPERTY(EditAnywhere, Category = Wwise)
		UAkAudioEvent* LandEvent;
	UPROPERTY(EditAnywhere, Category = Wwise)
		UAkAudioEvent* FootstepEvent;

	struct FTimerHandle FootTimer[2];

public:
	// Sets default values for this character's properties
	AEnemy_Archer();

protected:
	// Overrides
	virtual void BeginPlay() override; 
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
	virtual void Tick(float DeltaTime) override;
	virtual void Landed(const FHitResult& Hit) override;

	/* Combat */
	// Attacks
	UFUNCTION()
		void Attack(int32 AttackIndex);
	
	UFUNCTION()
		void ShootArrow();
	UFUNCTION()
		void ShootArrowHigh();
	UFUNCTION()
		void ShootArrowLow();

	// Directional dash methods
	void Dash();
	void StopDash();
	void ResetDash();

	void DisableIFrames();

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
	
	// functions to be bound to events
	/*UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);*/


};
