// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"

// Kismet
#include "Kismet/GameplayStatics.h"

// Components
#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

// Wwise
#include "AkGameplayStatics.h"
#include "AkComponent.h"

// My headers
#include "AttackHitbox.h"
#include "OptionsSaveGame.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	AudioComp = CreateDefaultSubobject<UAkComponent>(TEXT("Audio Comp"));
	//AudioComp = NewObject<UAkComponent>(this, TEXT("AudioComp"));
	//AudioComp->RegisterComponent();
	AudioComp->OcclusionCollisionChannel = ECollisionChannel::ECC_Visibility;
	AudioComp->OcclusionRefreshInterval = 0.1f;
	AudioDevice = FAkAudioDevice::Get();

	// Use only Yaw from the controller and ignore the rest of the rotation.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	HitBox = GetCapsuleComponent();

	FlipbookComponent = GetSprite();

	MoveComp = GetCharacterMovement();

	// Lock character motion onto the XZ plane, so the character can't move in or out of the screen
	MoveComp->bConstrainToPlane = true;
	MoveComp->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));

	MoveComp->bOrientRotationToMovement = false;

	// Behave like a traditional 2D platformer character, with a flat bottom instead of a curved capsule bottom
	// Note: This can cause a little floating when going up inclines; you can choose the tradeoff between better
	// behavior on the edge of a ledge versus inclines by setting this to true or false
	MoveComp->bUseFlatBaseForFloorChecks = true;

	UOptionsSaveGame* OptionsSaveGame;
	UGameplayStatics::DoesSaveGameExist("SettingsSlot", 0) ?
		OptionsSaveGame = Cast<UOptionsSaveGame>(UGameplayStatics::LoadGameFromSlot("SettingsSlot", 0)) :
		OptionsSaveGame = Cast<UOptionsSaveGame>(UGameplayStatics::CreateSaveGameObject(UOptionsSaveGame::StaticClass()));
	bUseAltAnims = OptionsSaveGame->GetEnableHighVisMode();

	// Defaults
	Health.CurrentPoints = Health.MaxPoints = 100.f;
	Health.PercentageRemaining = 1.f;	// 0 <= % <= 1
	Stamina.CurrentPoints = Stamina.MaxPoints = 100.f;
	Stamina.PercentageRemaining = 1.f;	// 0 <= % <= 1
	StaminaRegenRate = 25.f;

	bIsAnimInterruptible = true;
}

bool ACharacterBase::AmIDead()
{
	if (FMath::IsNearlyZero(Health.PercentageRemaining, 0.0001f)) {
		return true;
	}
	else {
		return false;
	}
}

void ACharacterBase::UpdateCharacter()
{
}

void ACharacterBase::UpdateAnimation()
{
}

void ACharacterBase::PlayOneShotAnim(UPaperFlipbook* Anim)
{
	FlipbookComponent->SetLooping(false);
	FlipbookComponent->SetFlipbook(Anim);
	FlipbookComponent->Play();
}

void ACharacterBase::TakeHit(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
}

void ACharacterBase::ResetFlipbook()
{
	if (!bDeathSequenceStarted) {
		bIsAnimInterruptible = true;
		FlipbookComponent->SetLooping(true);

		AAIController* Ctrl = Cast<AAIController>(GetController());
		if (Ctrl) {
			Ctrl->GetBlackboardComponent()->SetValueAsBool(FName("bIsHurtAnimPlaying"), false);
			Ctrl->GetBlackboardComponent()->SetValueAsBool(FName("bSpawnAnimFinished"), true);
		}
	}
}

void ACharacterBase::UseAltAnims(bool Val)
{
	bUseAltAnims = Val;
}

float ACharacterBase::GetHealth()
{
	return Health.PercentageRemaining;
}

float ACharacterBase::GetHealthVal()
{
	return Health.CurrentPoints;
}

void ACharacterBase::UpdateHealth(const float InHealthChange)
{
	// update current, clamp 0 <= current <= max, percentage calc
	Health.CurrentPoints += InHealthChange;
	Health.CurrentPoints = FMath::Clamp(Health.CurrentPoints, 0.f, Health.MaxPoints);
	Health.PercentageRemaining = Health.CurrentPoints / Health.MaxPoints;
}

float ACharacterBase::GetStamina()
{
	return Stamina.PercentageRemaining;
}

void ACharacterBase::UpdateStamina(const float InStaminaChange)
{
	// update current, clamp 0 <= current <= max, percentage calc
	Stamina.CurrentPoints += InStaminaChange;
	Stamina.CurrentPoints = FMath::Clamp(Stamina.CurrentPoints, 0.f, Stamina.MaxPoints);
	Stamina.PercentageRemaining = Stamina.CurrentPoints / Stamina.MaxPoints;
}
