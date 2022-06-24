// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CharacterBase.h"
#include "PlayerCharacter.generated.h"

DECLARE_DELEGATE_OneParam(FQuickHotbarDelegate, const int32);

UCLASS()
class CMP400_2DSOULSLIKE_API APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()

protected:
	/* Component Pointers */
	// Side view camera 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		USceneComponent* ListenerPosition;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* SideViewCameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class ASoulslikePlayerController* PlayerController;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = StatusEffects, meta = (AllowPrivateAccess = "true"))
	//	class UStatusEffectComponent* StatusEffectComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI, meta = (AllowPrivateAccess = "true"))
		class UAIPerceptionSystem* PercSys;

	/* UI Data */

	/* Gameplay Data */
	// Dash
	bool bCanDashInAir;
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
	
	// Attacks
	bool bAttacking;
	bool bSaveAttack;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Attacks)
		float MeleeDamage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Attacks)
		float MeleeCost;
	int32 AttackCount;
	struct FTimerHandle AttackTimer;
	class AAttackHitbox* MeleeHitbox;

	// IFrames
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = IFrames)
		bool bCanTakeDamage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = IFrames)
		struct FTimerHandle IFrameTimer;

	// Jumps
	int32 JumpCount;

	// Death
	bool bDeathSequenceEnded;

	// Interactable
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Interactable)
	class AInteractableBase* InteractableObject;

	// Animations
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animations, meta = (AllowPrivateAccess = "true"))
		EAnimState AnimState;

	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* AttackAnim[3];
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* AttackStopAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* CastAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* DashAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* DeathAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* FallAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* IdleAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* JumpAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* LandAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* NoBlurDashAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* RunAnim;

	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* AltAttackAnim[3];
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* AltAttackStopAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* AltCastAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* AltDashAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* AltDeathAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* AltFallAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* AltIdleAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* AltJumpAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* AltLandAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* AltNoBlurDashAnim;
	UPROPERTY(EditAnywhere, Category = Animations)
		UPaperFlipbook* AltRunAnim;

	// Audio
	UPROPERTY(EditAnywhere, Category = Wwise)
		UAkAudioEvent* AttackEvent[3];
	UPROPERTY(EditAnywhere, Category = Wwise)
		UAkAudioEvent* DashEvent;
	UPROPERTY(EditAnywhere, Category = Wwise)
		UAkAudioEvent* JumpEvent;
	UPROPERTY(EditAnywhere, Category = Wwise)
		UAkAudioEvent* LandEvent;
	UPROPERTY(EditAnywhere, Category = Wwise)
		UAkAudioEvent* FootstepEvent;

	struct FTimerHandle FootTimer[2];
	
	// Analytics
	FString LastDamageCauser;
	float NewQueueItemTimer;
	TArray<FVector>PositionQueue;

public:
	APlayerCharacter();

protected:
	// Overridden methods
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController) override;

	/* Movement Controls */
	// Called for side to side input
	void MoveRight(float Value);

	// Jump input
	virtual void Jump() override;
	virtual void StopJumping() override;
	virtual void Landed(const FHitResult& Hit) override;

	// Directional dash methods
	void Dash();
	void StopDash();
	void ResetDash();

	/* Combat Controls */
	// Attacks
	void MeleeAttack();
	void AttackTriggered();
	UFUNCTION()
		void DamageEnemies();
	void ComboAttack();
	void ResetCombo();

	void DisableIFrames();

	/* Interaction */
	// Interact/Speak/Read
	void Interact();

	/* UI Controls */
	//virtual void PauseGame();
	virtual void SetHotbarSlot(int32 SlotIndex);
	
	/* Anims */
	void UpdateCharacter();
	void UpdateAnimation();

	/*Audio*/
	UFUNCTION()
		void PlaySfx(UAkAudioEvent* SfxToPlay);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/* UI Controls */
		//virtual void UnpauseGame();

	void SetInteractable(class AInteractableBase* Interactable);
	FORCEINLINE class AInteractableBase* GetInteractableObject() const { return InteractableObject; }
	FORCEINLINE class ASoulslikePlayerController* GetPlayerController() const{ return PlayerController; }

	// Method to be bound to event
	UFUNCTION()
		virtual void TakeHit(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser) override;

	FORCEINLINE bool IsDeathSequenceFinished() const { return bDeathSequenceEnded; }
	
	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

};
