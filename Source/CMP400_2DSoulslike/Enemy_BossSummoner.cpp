// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_BossSummoner.h"

// Paper 2D
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "PaperSprite.h"

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
#include "GameFramework/GameModeBase.h"

// AI
//#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

// Navigation
#include "NavAreas/NavArea_Obstacle.h"

// My headers
#include "EnemyController_BossSummoner.h"
#include "Enemy_Wisp.h"
#include "PlayerCharacter.h"
#include "PlayerHUD.h"

#include "AttackHitbox.h"

AEnemy_BossSummoner::AEnemy_BossSummoner()
{
	// Set the size of our collision capsule.
	HitBox->SetCapsuleSize(8.f, 18.f, true);
	HitBox->SetRelativeScale3D(FVector(5.f, 5.f, 5.f));
	//HitBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	FlipbookComponent->SetRelativeLocation(FVector(2.f, 0.f, 2.5f));
	FlipbookComponent->SetRelativeScale3D(FVector(0.75f));
	FlipbookComponent->OnFinishedPlaying.AddDynamic(this, &AEnemy_BossSummoner::ResetFlipbook);

	// Damage Event
	OnTakeAnyDamage.AddDynamic(this, &AEnemy_BossSummoner::TakeHit);

	// AI
	//BehaviorTree = Cast<UBehaviorTree>(StaticLoadObject(UBehaviorTree::StaticClass(), nullptr, TEXT("BehaviorTree'/Game/AI/EvilWizard/EvilWizard_BT.EvilWizard_BT'")));
	AutoPossessPlayer = EAutoReceiveInput::Disabled;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AEnemyController_BossSummoner::StaticClass();

	AIAttackDelegate.AddDynamic(this, &AEnemy_BossSummoner::Attack);
	AITargetedAttackDelegate.AddDynamic(this, &AEnemy_BossSummoner::SummonWisps);
	AIAttackDelegateNoParam.AddDynamic(this, &AEnemy_BossSummoner::Heal);

	// Get all animation flipbooks
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>Attack1FlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Executioner/Boss_Attack1.Boss_Attack1'"));
	AttackAnim[0] = Attack1FlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>Attack2FlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Executioner/Boss_Attack2.Boss_Attack2'"));
	AttackAnim[1] = Attack2FlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>DeathFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Executioner/Boss_Death.Boss_Death'"));
	DeathAnim = DeathFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>HealFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Executioner/Boss_Skill.Boss_Skill'"));
	HealAnim = HealFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>IdleFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Executioner/Boss_Idle1.Boss_Idle1'"));
	IdleAnim = IdleFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>RunFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Executioner/Boss_Idle2.Boss_Idle2'"));
	RunAnim = RunFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>SummonFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Executioner/Boss_Summon.Boss_Summon'"));
	SummonAnim = SummonFlipbookAsset.Object;

	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AltAttack1FlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Executioner/HV_Boss_Attack1.HV_Boss_Attack1'"));
	AltAttackAnim[0] = AltAttack1FlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AltAttack2FlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Executioner/HV_Boss_Attack2.HV_Boss_Attack2'"));
	AltAttackAnim[1] = AltAttack2FlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AltDeathFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Executioner/HV_Boss_Death.HV_Boss_Death'"));
	AltDeathAnim = AltDeathFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AltHealFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Executioner/HV_Boss_Skill.HV_Boss_Skill'"));
	AltHealAnim = AltHealFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AltIdleFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Executioner/HV_Boss_Idle1.HV_Boss_Idle1'"));
	AltIdleAnim = AltIdleFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AltRunFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Executioner/HV_Boss_Idle2.HV_Boss_Idle2'"));
	AltRunAnim = AltRunFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AltSummonFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Executioner/HV_Boss_Summon.HV_Boss_Summon'"));
	AltSummonAnim = AltSummonFlipbookAsset.Object;

	//Wwise
	static ConstructorHelpers::FObjectFinder<UAkAudioEvent>AttackEvent0Asset(TEXT("AkAudioEvent'/Game/WwiseData/Events/PlayerCharacter/Play_Attack_01.Play_Attack_01'"));
	AttackEvent[0] = AttackEvent0Asset.Object;
	static ConstructorHelpers::FObjectFinder<UAkAudioEvent>AttackEvent1Asset(TEXT("AkAudioEvent'/Game/WwiseData/Events/PlayerCharacter/Play_Attack_02.Play_Attack_02'"));
	AttackEvent[1] = AttackEvent1Asset.Object;
	static ConstructorHelpers::FObjectFinder<UAkAudioEvent>CastEventAsset(TEXT("AkAudioEvent'/Game/WwiseData/Events/Troll/Play_TrollCast.Play_TrollCast'"));
	CastEvent = CastEventAsset.Object;

	// Configure character movement
	MoveComp->GravityScale = 0.f;
	MoveComp->GroundFriction = 3.f;
	MoveComp->BrakingFrictionFactor = 2.f;
	MoveComp->NavAgentProps.bCanFly = true;
	MoveComp->SetMovementMode(EMovementMode::MOVE_Flying);
	MoveComp->MaxFlySpeed = 750.0f;
	MoveComp->BrakingDecelerationFlying = 10.f;
	JumpMaxCount = 0;
	JumpMaxHoldTime = 0.f;

	//StatusEffectComp = CreateDefaultSubobject<UStatusEffectComponent>("Status Effect Comp");

	// Enable replication on the Sprite component so animations show up when networked
	GetSprite()->SetIsReplicated(true);
	bReplicates = true;

	// Set default animation state
	AnimState = EAnimState::EAS_Idle;

	// Defaults
	Health.CurrentPoints = Health.MaxPoints = 200.f;

	bAttacking = false;
	bSaveAttack = false;
	MeleeDamage = 20.f;

	SummonCooldown = 20.f;
	SummonTimer = 0.f;
	bCanSummon = true;
	
	HealPower = 40.f;
	HealCooldown = 20.f;
	HealTimer = 0.f;
	bCanHeal = true;

	bDeathSequenceStarted = false;
	DeathTimer = 0.f;
}

void AEnemy_BossSummoner::BeginPlay()
{
	Super::BeginPlay();

	// Hitbox
	MeleeHitbox = GetWorld()->SpawnActor<AAttackHitbox>();
	MeleeHitbox->SetupHitbox(140.f, this, MeleeDamage);
	MeleeHitbox->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	MeleeHitbox->SetActorRelativeLocation(FVector(15.f, 0.f, 6.f));
	MeleeHitbox->SetActorRelativeScale3D(FVector(0.2f));
}

void AEnemy_BossSummoner::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	AudioDevice->StopGameObject(AudioComp);
	MeleeHitbox->Destroy();
}

void AEnemy_BossSummoner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bCanSummon) {
		SummonTimer += DeltaTime;
		if (SummonTimer >= SummonCooldown) {
			bCanSummon = true;
			SummonTimer = 0.f;
		}
	}

	if (!bCanHeal) {
		HealTimer += DeltaTime;
		if (HealTimer >= HealCooldown) {
			bCanHeal = true;
			HealTimer = 0.f;
		}
	}

	if (AmIDead() && !bDeathSequenceStarted) {
		bDeathSequenceStarted = true;
		bSaveAttack = false;
		//GetWorld()->GetGameState<ADOOMCloneGameState>()->OnEnemyKilled.Broadcast();
		Cast<AEnemyController_BossSummoner>(GetController())->GetBlackboardComponent()->SetValueAsBool(FName("bDeathSequenceStarted"), true);

		bIsAnimInterruptible = false;
		if (bUseAltAnims) {
			PlayOneShotAnim(AltDeathAnim);
		}
		else {
			PlayOneShotAnim(DeathAnim);
		}
	}

	if (bDeathSequenceStarted && DeathTimer < DeathAnim->GetTotalDuration()) {
		//for (UMaterialInterface* Material : CharacterMesh->GetMaterials()) {
		//	UMaterialInstanceDynamic* DynamicMat = Cast<UMaterialInstanceDynamic>(Material);
		//	//UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Material Name: %s"), *Material->GetName()), true, true, FLinearColor(FColor::Cyan), 2.f);
		//	if (DynamicMat)
		//		DynamicMat->SetScalarParameterValue(FName("Appearance"), DeathTimer);
		//}
		DeathTimer += DeltaTime;
	}
	else if (bDeathSequenceStarted && DeathTimer >= DeathAnim->GetTotalDuration()) {
		Destroy();
	}

	UpdateCharacter();
}

void AEnemy_BossSummoner::Attack(int32 AttackIndex)
{
	if (bAttacking) {
		bSaveAttack = true;
		return;
	}
	else {
		AttackTriggered();
	}
}

void AEnemy_BossSummoner::AttackTriggered()
{
	if (bIsAnimInterruptible) {

		GetWorldTimerManager().ClearTimer(AttackTimer);
		GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy_BossSummoner::ComboAttack, AttackAnim[AttackCount]->GetTotalDuration(), false);


		FTimerHandle AttackHitboxTimer;
		FTimerDelegate DamageDelegate;
		DamageDelegate.BindUFunction(this, "DamageEnemies");
		switch (AttackCount) {
		case 0:
			MeleeHitbox->SetActorRelativeLocation(FVector(15.f, 0.f, 6.f));
			GetWorldTimerManager().SetTimer(AttackHitboxTimer, DamageDelegate, 6.f / 24.f, false);
			break;
		case 1:
			MeleeHitbox->SetActorRelativeLocation(FVector(14.f, 0.f, 1.f));
			GetWorldTimerManager().SetTimer(AttackHitboxTimer, DamageDelegate, 8.f / 24.f, false);
			break;
		}

		bIsAnimInterruptible = false;
		if (bUseAltAnims) {
			PlayOneShotAnim(AltAttackAnim[AttackCount]);
		}
		else {
			PlayOneShotAnim(AttackAnim[AttackCount]);
		}
		PlaySfx(AttackEvent[AttackCount]);

		AttackCount++;

		if (AttackCount > 1)
			AttackCount = 0;

		bSaveAttack = false;
		bAttacking = true;
	}
	else {
		ResetCombo();
	}
}

void AEnemy_BossSummoner::DamageEnemies()
{
	MeleeHitbox->DamageEnemies();
}

void AEnemy_BossSummoner::ComboAttack()
{
	bIsAnimInterruptible = true;
	if (bSaveAttack) {
		AttackTriggered();
	}
	else {
		ResetCombo();
	}
}

void AEnemy_BossSummoner::ResetCombo()
{
	AttackCount = 0;
	bSaveAttack = false;
	bAttacking = false;
}

void AEnemy_BossSummoner::SummonWisps(const AActor* Target)
{
	MoveComp->StopMovementImmediately();

	FVector Loc = Target->GetActorLocation() + FVector(-100.f, 0.f, 100.f);
	GetWorld()->SpawnActor<AEnemy_Wisp>(Loc, GetActorRotation());
	Loc = Target->GetActorLocation() + FVector(-50.f, 0.f, 175.f);
	GetWorld()->SpawnActor<AEnemy_Wisp>(Loc, GetActorRotation());
	Loc = Target->GetActorLocation() + FVector(50.f, 0.f, 175.f);
	GetWorld()->SpawnActor<AEnemy_Wisp>(Loc, GetActorRotation());
	Loc = Target->GetActorLocation() + FVector(100.f, 0.f, 100.f);
	GetWorld()->SpawnActor<AEnemy_Wisp>(Loc, GetActorRotation());

	bIsAnimInterruptible = false;
	if (bUseAltAnims) {
		PlayOneShotAnim(AltHealAnim);
	}
	else {
		PlayOneShotAnim(HealAnim);
	}
	PlaySfx(CastEvent);
	bCanSummon = false;

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Magenta, FString::Printf(TEXT("Boss summoned help")));
}

void AEnemy_BossSummoner::Heal()
{
	MoveComp->StopMovementImmediately();
	bIsAnimInterruptible = false;
	if (bUseAltAnims) {
		PlayOneShotAnim(AltHealAnim);
	}
	else {
		PlayOneShotAnim(HealAnim);
	}
	PlaySfx(CastEvent);
	bCanHeal = false;
	UpdateHealth(HealPower);

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, FString::Printf(TEXT("Boss healed, health = %f"), Health.CurrentPoints));
}

void AEnemy_BossSummoner::UpdateCharacter()
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
}

void AEnemy_BossSummoner::UpdateAnimation()
{
	const FVector Velocity = GetVelocity();

	EAnimState NotMoving = (Velocity.Size() != 0) ? EAnimState::EAS_Running : EAnimState::EAS_Idle;
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

void AEnemy_BossSummoner::PlaySfx(UAkAudioEvent* SfxToPlay, bool bTriggerSfxOnCompass)
{
	//AudioDevice->PostEvent(SfxToPlay, this);
	AudioComp->PostAkEvent(SfxToPlay, 0, FOnAkPostEventCallback(), TArray<FAkExternalSourceInfo>(), SfxToPlay->GetFName().ToString());
	if (bTriggerSfxOnCompass)
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD<APlayerHUD>()->SfxTriggered(GetActorLocation(), 0.75f);
}

void AEnemy_BossSummoner::TakeHit(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (!bDeathSequenceStarted) {
		bSaveAttack = false;

		//if (DamageType->GetClass()->ImplementsInterface(UDamageTypeInterface::StaticClass())) {
		//	IDamageTypeInterface* DamageInterface = (IDamageTypeInterface*)Cast<IDamageTypeInterface>(DamageType);
		//	if (DamageInterface)
		//		DamageInterface->TriggerEffect(StatusEffectComp);
		//}

		MoveComp->AddImpulse((GetActorLocation() - DamageCauser->GetActorLocation()).GetSafeNormal() * 500.f, true);

		// check damage
		if (Damage < 0.f)
			return;

		UpdateHealth(-Damage);
		
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Boss hit, health = %f"), Health.CurrentPoints));
	}
}