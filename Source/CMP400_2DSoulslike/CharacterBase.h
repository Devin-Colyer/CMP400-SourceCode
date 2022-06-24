// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PaperCharacter.h"
#include "CharacterBase.generated.h"

class UPaperFlipbook;
class UAkAudioEvent;
class UAkComponent;

UENUM(BlueprintType)
enum class EAnimState : uint8 {
	EAS_Idle UMETA(DisplayName = "Idle"),
	EAS_Running UMETA(DisplayName = "Running"),
	EAS_Falling UMETA(DisplayName = "Falling"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	EAS_Unknown
};

// Generic struct containing health points, ammo, etc
USTRUCT(BlueprintType)
struct FGamePoints {
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float MaxPoints;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float CurrentPoints;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float PercentageRemaining;

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAIAttackDelegateNoParam);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAIAttackDelegate, int32, AttackIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAITargetedAttackDelegate, const AActor*, Target);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAIStartAttackDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAIStopAttackDelegate);

UCLASS(config = Game)
class CMP400_2DSOULSLIKE_API ACharacterBase : public APaperCharacter
{
	GENERATED_BODY()

protected:
	/* Component pointers */
	// Character movement
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class UCharacterMovementComponent* MoveComp;

	// Character hitbox
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent* HitBox;

	// Character visuals
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animations, meta = (AllowPrivateAccess = "true"))
		class UPaperFlipbookComponent* FlipbookComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animations, meta = (AllowPrivateAccess = "true"))
		bool bUseAltAnims;

	// Audio
	UPROPERTY(EditAnywhere, Category = Wwise)
		UAkComponent* AudioComp;
	class FAkAudioDevice* AudioDevice;

	/* Gameplay Data */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayData)
		FGamePoints Health;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayData)
		FGamePoints Stamina;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayData)
		float StaminaRegenRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combo)
		bool bIsAnimInterruptible;

	// Death
	bool bDeathSequenceStarted;
	float DeathTimer;

public:
	ACharacterBase();

	UPROPERTY(BlueprintCallable, Category = AI)
		FAIAttackDelegateNoParam AIAttackDelegateNoParam;
	UPROPERTY(BlueprintCallable, Category = AI)
		FAIAttackDelegate AIAttackDelegate;
	UPROPERTY(BlueprintCallable, Category = AI)
		FAITargetedAttackDelegate AITargetedAttackDelegate;
	UPROPERTY(BlueprintCallable, Category = AI)
		FAIStartAttackDelegate AIStartAttackDelegate;
	UPROPERTY(BlueprintCallable, Category = AI)
		FAIStopAttackDelegate AIStopAttackDelegate;

protected:
	virtual bool AmIDead();

	// Called to choose the correct animation to play based on the character's movement state
	virtual void UpdateCharacter();
	virtual void UpdateAnimation();
	virtual void PlayOneShotAnim(class UPaperFlipbook* Anim);

	virtual void TakeHit(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

public:

	/* Utility */
	UFUNCTION()
		virtual void ResetFlipbook();
	UFUNCTION()
		virtual void UseAltAnims(bool Val);
	UFUNCTION()
		inline bool UsingAltAnims() { return bUseAltAnims; }

	// Health getters & update
	UFUNCTION(BlueprintPure, Category = Health)
		float GetHealth();
	UFUNCTION(BlueprintPure, Category = Health)
		float GetHealthVal();
	UFUNCTION(BlueprintCallable, Category = Health)
		void UpdateHealth(const float InHealthChange);
	UFUNCTION(BlueprintPure, Category = Stamina)
		float GetStamina();
	UFUNCTION(BlueprintCallable, Category = Stamina)
		void UpdateStamina(const float InStaminaChange);

	/** Returns FlipbookComponent subobject **/
	FORCEINLINE class UPaperFlipbookComponent* GetPlayerSprite() const { return FlipbookComponent; }

};
