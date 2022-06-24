// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Troll.h"

// Paper 2D
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"

// AkAudio - Wwise
#include "AkAudioEvent.h"
#include "AkComponent.h"

// Components
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"

// Helpers
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

// AI
//#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

// Navigation
#include "NavAreas/NavArea_Obstacle.h"

// My headers
#include "EnemyController_Troll.h"
#include "TrollSpellEffect.h"
#include "AttackHitbox.h"
#include "PlayerCharacter.h"
#include "PlayerHUD.h"

AEnemy_Troll::AEnemy_Troll()
{
	// Set the size of our collision capsule.
	HitBox->SetCapsuleSize(9.f, 21.f, true);
	HitBox->SetRelativeScale3D(FVector(5.f, 5.f, 5.f));

	FlipbookComponent->SetRelativeLocation(FVector(26.f, 0.f, 14.f));
	FlipbookComponent->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
	FlipbookComponent->SetRelativeScale3D(FVector(0.75f));
	FlipbookComponent->OnFinishedPlaying.AddDynamic(this, &AEnemy_Troll::ResetFlipbook);

	// Damage Event
	OnTakeAnyDamage.AddDynamic(this, &AEnemy_Troll::TakeHit);

	// AI
	//BehaviorTree = Cast<UBehaviorTree>(StaticLoadObject(UBehaviorTree::StaticClass(), nullptr, TEXT("BehaviorTree'/Game/AI/EvilWizard/EvilWizard_BT.EvilWizard_BT'")));
	AutoPossessPlayer = EAutoReceiveInput::Disabled;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AEnemyController_Troll::StaticClass();

	AIAttackDelegate.AddDynamic(this, &AEnemy_Troll::Slam);
	AITargetedAttackDelegate.AddDynamic(this, &AEnemy_Troll::CastAttack);

	// Get all animation flipbooks
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AttackFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Clembod_BringerOfDeath/BoD_Attack.BoD_Attack'"));
	AttackAnim[0] = AttackFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>CastFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Clembod_BringerOfDeath/BoD_Cast.BoD_Cast'"));
	AttackAnim[1] = CastFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>DeathFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Clembod_BringerOfDeath/BoD_Death.BoD_Death'"));
	DeathAnim = DeathFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>IdleFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Clembod_BringerOfDeath/BoD_Idle.BoD_Idle'"));
	IdleAnim = IdleFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>RunFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Clembod_BringerOfDeath/BoD_Walk.BoD_Walk'"));
	RunAnim = RunFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>TakeHitFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Clembod_BringerOfDeath/BoD_Hurt.BoD_Hurt'"));
	HurtAnim = TakeHitFlipbookAsset.Object;

	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AltAttackFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Clembod_BringerOfDeath/HV_BoD_Attack.HV_BoD_Attack'"));
	AltAttackAnim[0] = AltAttackFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AltCastFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Clembod_BringerOfDeath/HV_BoD_Cast.HV_BoD_Cast'"));
	AltAttackAnim[1] = AltCastFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AltDeathFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Clembod_BringerOfDeath/HV_BoD_Death.HV_BoD_Death'"));
	AltDeathAnim = AltDeathFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AltIdleFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Clembod_BringerOfDeath/HV_BoD_Idle.HV_BoD_Idle'"));
	AltIdleAnim = AltIdleFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AltRunFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Clembod_BringerOfDeath/HV_BoD_Walk.HV_BoD_Walk'"));
	AltRunAnim = AltRunFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AltTakeHitFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Clembod_BringerOfDeath/HV_BoD_Hurt.HV_BoD_Hurt'"));
	AltHurtAnim = AltTakeHitFlipbookAsset.Object;

	//Wwise
	static ConstructorHelpers::FObjectFinder<UAkAudioEvent>AttackEventAsset(TEXT("AkAudioEvent'/Game/WwiseData/Events/Troll/Play_TrollSwing.Play_TrollSwing'"));
	AttackEvent = AttackEventAsset.Object;
	static ConstructorHelpers::FObjectFinder<UAkAudioEvent>CastEventAsset(TEXT("AkAudioEvent'/Game/WwiseData/Events/Troll/Play_TrollCast.Play_TrollCast'"));
	CastEvent = CastEventAsset.Object;
	static ConstructorHelpers::FObjectFinder<UAkAudioEvent>LandEventAsset(TEXT("AkAudioEvent'/Game/WwiseData/Events/PlayerCharacter/Play_Land.Play_Land'"));
	LandEvent = LandEventAsset.Object;
	static ConstructorHelpers::FObjectFinder<UAkAudioEvent>FootstepEventAsset(TEXT("AkAudioEvent'/Game/WwiseData/Events/Troll/Play_Footsteps_02.Play_Footsteps_02'"));
	FootstepEvent = FootstepEventAsset.Object;

	// Configure character movement
	MoveComp->GravityScale = 2.0f;
	MoveComp->GroundFriction = 3.0f;
	MoveComp->BrakingFrictionFactor = 2.f;
	MoveComp->MaxWalkSpeed = 300.0f;
	//MoveComp->MaxFlySpeed = 600.0f;
	JumpMaxCount = 0;
	JumpMaxHoldTime = 0.f;

	//StatusEffectComp = CreateDefaultSubobject<UStatusEffectComponent>("Status Effect Comp");

	// Enable replication on the Sprite component so animations show up when networked
	GetSprite()->SetIsReplicated(true);
	bReplicates = true;

	// Set default animation state
	AnimState = EAnimState::EAS_Idle;

	// Defaults
	Health.CurrentPoints = Health.MaxPoints = 120.f;
	StaminaRegenRate = 20.f;

	bAttacking = false;
	bSaveAttack = false;
	SlamDamage = 50.f;
	SlamCost = 33.f;
	CastDamage = 30.f;
	CastCost = 20.f;
	StaminaRegenRate = 20.f;

	bDeathSequenceStarted = false;
	DeathTimer = 0.f;
}

void AEnemy_Troll::BeginPlay()
{
	Super::BeginPlay();

	// Hitbox
	SlamHitbox = GetWorld()->SpawnActor<AAttackHitbox>();
	SlamHitbox->SetupHitbox(FVector(1.f), this, SlamDamage);
	SlamHitbox->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	SlamHitbox->SetActorRelativeScale3D(FVector(0.2f));
	SlamHitbox->SetActorRelativeLocation(FVector(44.f, 0.f, 3.f));
	SlamHitbox->ChangeShape(FVector(175.f, 32.f, 115.f));
}

void AEnemy_Troll::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	AudioDevice->StopGameObject(AudioComp);
	SlamHitbox->Destroy();
}

void AEnemy_Troll::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bAttacking && !MoveComp->IsFalling())
		UpdateStamina(StaminaRegenRate * DeltaTime);

	if (AmIDead() && !bDeathSequenceStarted) {
		bDeathSequenceStarted = true;
		//GetWorld()->GetGameState<ADOOMCloneGameState>()->OnEnemyKilled.Broadcast();
		 
		Cast<AEnemyController_Troll>(GetController())->GetBlackboardComponent()->SetValueAsBool(FName("bDeathSequenceStarted"), true);

		bIsAnimInterruptible = false;
		if (bUseAltAnims) {
			PlayOneShotAnim(AltDeathAnim);
		}
		else {
			PlayOneShotAnim(DeathAnim);
		}
	}

	if (bDeathSequenceStarted && DeathTimer < DeathAnim->GetTotalDuration())
		DeathTimer += DeltaTime;
	else if (bDeathSequenceStarted && DeathTimer >= DeathAnim->GetTotalDuration())
		Destroy();

	UpdateCharacter();
}

void AEnemy_Troll::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	PlaySfx(LandEvent, false);
}

void AEnemy_Troll::Slam(int32 AttackIndex)
{
	if (bIsAnimInterruptible && Stamina.CurrentPoints > SlamCost) {
		bAttacking = true;
		UpdateStamina(-SlamCost);

		FTimerHandle AttackTimer;
		FTimerDelegate DamageDelegate;
		DamageDelegate.BindUFunction(this, "DamageEnemies");
		GetWorldTimerManager().SetTimer(AttackTimer, DamageDelegate, 13.f / 24.f, false);

		bIsAnimInterruptible = false;
		MoveComp->StopMovementImmediately();
		if (bUseAltAnims) {
			PlayOneShotAnim(AltAttackAnim[0]);
		}
		else {
			PlayOneShotAnim(AttackAnim[0]);
		}
		PlaySfx(AttackEvent, false);
	}
}

void AEnemy_Troll::DamageEnemies()
{
	if (bAttacking)
		SlamHitbox->DamageEnemies();

	bAttacking = false;
}

void AEnemy_Troll::CastAttack(const AActor* Target)
{
	if (bIsAnimInterruptible && Stamina.CurrentPoints > CastCost) {
		bAttacking = true;
		UpdateStamina(-CastCost);

		FTimerHandle AttackTimer;
		FTimerDelegate SpellDelegate;
		SpellDelegate.BindUFunction(this, "SpawnSpell", (AActor*)Target);
		GetWorldTimerManager().SetTimer(AttackTimer, SpellDelegate, 14.f / 24.f, false);

		bIsAnimInterruptible = false;
		MoveComp->StopMovementImmediately();
		if (bUseAltAnims) {
			PlayOneShotAnim(AltAttackAnim[1]);
		}
		else {
			PlayOneShotAnim(AttackAnim[1]);
		}
		PlaySfx(CastEvent, true);
	}
}

void AEnemy_Troll::SpawnSpell(AActor* Target)
{
	//y
	FVector SpellPos;
	FVector TargetSize;
	Target->GetActorBounds(true, SpellPos, TargetSize);
	SpellPos.Z += 169.f;
	SpellPos.Z -= (TargetSize.Z);

	ATrollSpellEffect* Spell = GetWorld()->SpawnActor<ATrollSpellEffect>(SpellPos, GetActorRotation());
	Spell->InitializeSpell(CastDamage, bUseAltAnims);

	bAttacking = false;
}

void AEnemy_Troll::UpdateCharacter()
{
	// Update animation to match the motion
	UpdateAnimation();

	// Now setup the rotation of the controller based on the direction we are travelling
	const FVector Velocity = GetVelocity();
	float TravelDirection = Velocity.X;
	// Set the rotation so that the character faces his direction of travel.
	if (Controller != nullptr)
	{
		if (TravelDirection < 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
		}
		else if (TravelDirection > 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
		}
	}

	if (FMath::IsNearlyZero(Velocity.X) || MoveComp->IsFalling()) {
		GetWorldTimerManager().ClearTimer(FootTimer[0]);
		GetWorldTimerManager().ClearTimer(FootTimer[1]);
	}
}

void AEnemy_Troll::UpdateAnimation()
{
	const FVector Velocity = GetVelocity();

	EAnimState GroundMovement = MoveComp->IsFalling() ? EAnimState::EAS_Idle : EAnimState::EAS_Running;
	EAnimState NotMoving = (Velocity.Size() != 0) ? GroundMovement : EAnimState::EAS_Idle;
	AnimState = NotMoving;

	if (bIsAnimInterruptible) {
		switch (AnimState) {
		case EAnimState::EAS_Idle:
			if (bUseAltAnims) {
				FlipbookComponent->SetFlipbook(AltIdleAnim);
			}
			else {
				FlipbookComponent->SetFlipbook(IdleAnim);
			}
			break;
		case EAnimState::EAS_Running:
			if (!FootTimer[0].IsValid() && !FootTimer[1].IsValid()) {
				float Delay[2] = { 2.f / 24.f, 8.f / 24.f };
				for (size_t i = 0; i < 2; i++)
				{
					FTimerDelegate SfxDelegate;
					SfxDelegate.BindUFunction(this, "PlaySfx", FootstepEvent, false);
					GetWorldTimerManager().ClearTimer(FootTimer[i]);
					GetWorldTimerManager().SetTimer(FootTimer[i], SfxDelegate, 12.f / 24.f, true, Delay[i]);
				}
			}
			if (bUseAltAnims) {
				FlipbookComponent->SetFlipbook(AltRunAnim);
			}
			else {
				FlipbookComponent->SetFlipbook(RunAnim);
			}
			break;
		default:
			break;
		}

		if (!FlipbookComponent->IsPlaying())
			FlipbookComponent->Play();
	}
}

void AEnemy_Troll::PlaySfx(UAkAudioEvent* SfxToPlay, bool bTriggerSfxOnCompass)
{
	AudioComp->PostAkEvent(SfxToPlay, 0, FOnAkPostEventCallback(), TArray<FAkExternalSourceInfo>(), SfxToPlay->GetFName().ToString());
	if (bTriggerSfxOnCompass)
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD<APlayerHUD>()->SfxTriggered(GetActorLocation(), 1.f);
}

void AEnemy_Troll::TakeHit(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (!bDeathSequenceStarted) {
		UpdateStamina(-(Damage / 2.f));
		bAttacking = false;

		bIsAnimInterruptible = false;
		if (bUseAltAnims) {
			PlayOneShotAnim(AltHurtAnim);
		}
		else {
			PlayOneShotAnim(HurtAnim);
		}

		Cast<AEnemyController_Troll>(GetController())->GetBlackboardComponent()->SetValueAsBool(FName("bIsHurtAnimPlaying"), true);

		//if (DamageType->GetClass()->ImplementsInterface(UDamageTypeInterface::StaticClass())) {
		//	IDamageTypeInterface* DamageInterface = (IDamageTypeInterface*)Cast<IDamageTypeInterface>(DamageType);
		//	if (DamageInterface)
		//		DamageInterface->TriggerEffect(StatusEffectComp);
		//}

		// check damage
		if (Damage < 0.f)
			return;

		UpdateHealth(-Damage);
	}
}

