// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_EvilWizard.h"

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
#include "EnemyController_EvilWizard.h"
#include "PlayerCharacter.h"

AEnemy_EvilWizard::AEnemy_EvilWizard()
{
	// Set the size of our collision capsule.
	HitBox->SetCapsuleSize(7.5f, 18.f, true);
	HitBox->SetRelativeScale3D(FVector(5.f, 5.f, 5.f));

	FlipbookComponent->SetRelativeLocation(FVector(0.f, 0.f, 2.5f));
	FlipbookComponent->SetRelativeScale3D(FVector(0.75f));
	FlipbookComponent->OnFinishedPlaying.AddDynamic(this, &AEnemy_EvilWizard::ResetFlipbook);	
	
	FlameHitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("FlameHitbox"));
	FlameHitbox->InitBoxExtent(FVector(95.f, 32.f, 70.f));
	FlameHitbox->SetRelativeScale3D(FVector(0.2f));
	FlameHitbox->SetRelativeLocation(FVector(27.f, 0.f, 4.f));
	FlameHitbox->SetGenerateOverlapEvents(true);
	FlameHitbox->SetCollisionResponseToChannels(ECollisionResponse::ECR_Overlap);
	FlameHitbox->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	FlameHitbox->OnComponentEndOverlap.AddDynamic(this, &AEnemy_EvilWizard::OnOverlapEnd);

	// Damage Event
	OnTakeAnyDamage.AddDynamic(this, &AEnemy_EvilWizard::TakeHit);

	// AI
	//BehaviorTree = Cast<UBehaviorTree>(StaticLoadObject(UBehaviorTree::StaticClass(), nullptr, TEXT("BehaviorTree'/Game/AI/EvilWizard/EvilWizard_BT.EvilWizard_BT'")));
	AutoPossessPlayer = EAutoReceiveInput::Disabled;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AEnemyController_EvilWizard::StaticClass();

	AIStartAttackDelegate.AddDynamic(this, &AEnemy_EvilWizard::StartAttack);
	AIStopAttackDelegate.AddDynamic(this, &AEnemy_EvilWizard::StopAttack);

	// Get all animation flipbooks
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AttackFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/LuizMelo_EvilWizard/AttackAnim.AttackAnim'"));
	AttackAnim = AttackFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>DeathFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/LuizMelo_EvilWizard/DeathAnim.DeathAnim'"));
	DeathAnim = DeathFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>IdleFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/LuizMelo_EvilWizard/IdleAnim.IdleAnim'"));
	IdleAnim = IdleFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>RunFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/LuizMelo_EvilWizard/MoveAnim.MoveAnim'"));
	RunAnim = RunFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>TakeHitFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/LuizMelo_EvilWizard/TakeHitAnim.TakeHitAnim'"));
	HurtAnim = TakeHitFlipbookAsset.Object;

	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AltAttackFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/LuizMelo_EvilWizard/HV_AttackAnim.HV_AttackAnim'"));
	AltAttackAnim = AltAttackFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AltDeathFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/LuizMelo_EvilWizard/HV_DeathAnim.HV_DeathAnim'"));
	AltDeathAnim = AltDeathFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AltIdleFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/LuizMelo_EvilWizard/HV_IdleAnim.HV_IdleAnim'"));
	AltIdleAnim = AltIdleFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AltRunFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/LuizMelo_EvilWizard/HV_MoveAnim.HV_MoveAnim'"));
	AltRunAnim = AltRunFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AltTakeHitFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/LuizMelo_EvilWizard/HV_TakeHitAnim.HV_TakeHitAnim'"));
	AltHurtAnim = AltTakeHitFlipbookAsset.Object;

	// Wwise Events
	static ConstructorHelpers::FObjectFinder<UAkAudioEvent>StartAttackEventAsset(TEXT("AkAudioEvent'/Game/WwiseData/Events/Wizard/Play_Flamethrower.Play_Flamethrower'"));
	StartAttackEvent = StartAttackEventAsset.Object;
	static ConstructorHelpers::FObjectFinder<UAkAudioEvent>StopAttackEventAsset(TEXT("AkAudioEvent'/Game/WwiseData/Events/Wizard/Stop_Flamethrower.Stop_Flamethrower'"));
	StopAttackEvent = StopAttackEventAsset.Object;

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
	Health.CurrentPoints = Health.MaxPoints = 60.f;
	StaminaRegenRate = 20.f;

	bAttacking = false;
	bSaveAttack = false;
	FlameDamage = 5.f;
	FlameCost = 50; 
	FlameTickFrequency = 0.25f;
	bCanApplyDamage = false;

	bDeathSequenceStarted = false;
	DeathTimer = 0.f;
}


void AEnemy_EvilWizard::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemy_EvilWizard::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	//AudioComp->PostAkEvent(StopAttackEvent, 0, FOnAkPostEventCallback(), FString("Stop_Flamethrower"));
	//AudioDevice->PostEvent(StopAttackEvent, this);
	AudioDevice->StopGameObject(AudioComp);
}

void AEnemy_EvilWizard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bAttacking && !MoveComp->IsFalling())
		UpdateStamina(StaminaRegenRate * DeltaTime);

	if (AmIDead() && !bDeathSequenceStarted) {
		bDeathSequenceStarted = true;
		//GetWorld()->GetGameState<ADOOMCloneGameState>()->OnEnemyKilled.Broadcast();
		Cast<AEnemyController_EvilWizard>(GetController())->GetBlackboardComponent()->SetValueAsBool(FName("bDeathSequenceStarted"), true);

		bIsAnimInterruptible = false;
		if (bUseAltAnims) {
			PlayOneShotAnim(AltDeathAnim);
		}
		else {
			PlayOneShotAnim(DeathAnim);
		}
		StopAttack();
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
	
	// Damage if attacking
	if (bAttacking && !DamageTimer.IsValid()) {
		TArray<AActor*> OverlappingActors;
		FlameHitbox->GetOverlappingActors(OverlappingActors, APlayerCharacter::StaticClass());
		for (AActor* Actor : OverlappingActors) {
			if (Actor != this && !DamageTimer.IsValid()) {
				// Actor can apply damage, set member properties, bind function to timer
				bCanApplyDamage = true;
				PlayerCharacter = Actor;
				GetWorldTimerManager().SetTimer(DamageTimer, this, &AEnemy_EvilWizard::ApplyDamage, FlameTickFrequency, true, 0.f);
			}
		}
	}

	UpdateCharacter();
}

void AEnemy_EvilWizard::StartAttack()
{
	if (Stamina.CurrentPoints > FlameCost && !bAttacking) {
		UpdateStamina(-FlameCost);
		bAttacking = true;
		//AudioDevice->PostEvent(StartAttackEvent, this);
		PlaySfx(StartAttackEvent);
	}
}

void AEnemy_EvilWizard::StopAttack()
{
	bAttacking = false;
	bCanApplyDamage = false;
	GetWorldTimerManager().ClearTimer(DamageTimer);
	//AudioDevice->PostEvent(StopAttackEvent, this);
	PlaySfx(StopAttackEvent);
}

void AEnemy_EvilWizard::ApplyDamage()
{
	// if actor can apply damage call ApplyDamage event on player character
	if (bCanApplyDamage)
		UGameplayStatics::ApplyDamage(PlayerCharacter, FlameDamage, nullptr, this, UDamageType::StaticClass());
}

void AEnemy_EvilWizard::UpdateCharacter()
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

void AEnemy_EvilWizard::UpdateAnimation()
{
	const FVector Velocity = GetVelocity();

	EAnimState GroundMovement = MoveComp->IsFalling() ? EAnimState::EAS_Idle : EAnimState::EAS_Running;
	EAnimState NotMoving = (Velocity.Size() != 0) ? GroundMovement : EAnimState::EAS_Idle;
	EAnimState Attacking = bAttacking ? EAnimState::EAS_Attacking : GroundMovement;
	AnimState = Attacking;

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
		case EAnimState::EAS_Attacking:
			if (bUseAltAnims) {
				FlipbookComponent->SetFlipbook(AltAttackAnim);
			}
			else {
				FlipbookComponent->SetFlipbook(AttackAnim);
			}
			break;
		default:
			break;
		}

		if (!FlipbookComponent->IsPlaying())
			FlipbookComponent->Play();
	}
}

void AEnemy_EvilWizard::PlaySfx(UAkAudioEvent* SfxToPlay)
{
	AudioComp->PostAkEvent(SfxToPlay, 0, FOnAkPostEventCallback(), TArray<FAkExternalSourceInfo>(), SfxToPlay->GetFName().ToString());
}

void AEnemy_EvilWizard::TakeHit(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (!bDeathSequenceStarted) {
		UpdateStamina(-(Damage / 2.f));
		StopAttack();

		bIsAnimInterruptible = false;
		if (bUseAltAnims) {
			PlayOneShotAnim(AltHurtAnim);
		}
		else {
			PlayOneShotAnim(HurtAnim);
		}

		Cast<AEnemyController_EvilWizard>(GetController())->GetBlackboardComponent()->SetValueAsBool(FName("bIsHurtAnimPlaying"), true);

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

void AEnemy_EvilWizard::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// actor can't apply damage, unbind function from timer
	bCanApplyDamage = false;
	GetWorldTimerManager().ClearTimer(DamageTimer);
}
