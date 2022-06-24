// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Archer.h"

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
#include "EnemyController_Archer.h"
#include "PlayerCharacter.h"
#include "PlayerHUD.h"
#include "ProjectileBase.h"

#include "Enemy_EvilWizard.h"

AEnemy_Archer::AEnemy_Archer()
{
	// Set the size of our collision capsule.
	HitBox->SetCapsuleSize(7.f, 14.f, true);
	HitBox->SetRelativeScale3D(FVector(5.f, 5.f, 5.f));

	DodgeTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("DodgeTrigger"));
	DodgeTrigger->InitBoxExtent(FVector(100.f, 32.f, 50.f));
	DodgeTrigger->SetRelativeScale3D(FVector(0.2f));
	DodgeTrigger->SetRelativeLocation(FVector(0.f));
	DodgeTrigger->SetGenerateOverlapEvents(true);
	DodgeTrigger->SetCollisionResponseToChannels(ECollisionResponse::ECR_Overlap);
	DodgeTrigger->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	FlipbookComponent->SetRelativeLocation(FVector(0.f, 0.f, 2.5f));
	FlipbookComponent->SetRelativeScale3D(FVector(0.75f));
	FlipbookComponent->OnFinishedPlaying.AddDynamic(this, &AEnemy_Archer::ResetFlipbook);

	// Damage Event
	OnTakeAnyDamage.AddDynamic(this, &AEnemy_Archer::TakeHit);

	// AI
	//BehaviorTree = Cast<UBehaviorTree>(StaticLoadObject(UBehaviorTree::StaticClass(), nullptr, TEXT("BehaviorTree'/Game/AI/EvilWizard/EvilWizard_BT.EvilWizard_BT'")));
	AutoPossessPlayer = EAutoReceiveInput::Disabled;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AEnemyController_Archer::StaticClass();

	AIAttackDelegate.AddDynamic(this, &AEnemy_Archer::Attack);

	// Get all animation flipbooks
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AttackFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Archer/Archer_NormalAttack.Archer_NormalAttack'"));
	AttackAnim[0] = AttackFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AttackHighFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Archer/Archer_HighAttack.Archer_HighAttack'"));
	AttackAnim[1] = AttackHighFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AttackLowFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Archer/Archer_LowAttack.Archer_LowAttack'"));
	AttackAnim[2] = AttackLowFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>DashFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Archer/Archer_Dash.Archer_Dash'"));
	DashAnim = DashFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>DeathFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Archer/Archer_Death.Archer_Death'"));
	DeathAnim = DeathFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>IdleFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Archer/Archer_Idle.Archer_Idle'"));
	IdleAnim = IdleFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>RunFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Archer/Archer_Run.Archer_Run'"));
	RunAnim = RunFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>TakeHitFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Archer/Archer_Hurt.Archer_Hurt'"));
	HurtAnim = TakeHitFlipbookAsset.Object;

	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AltAttackFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Archer/HV_Archer_NormalAttack.HV_Archer_NormalAttack'"));
	AltAttackAnim[0] = AltAttackFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AltAttackHighFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Archer/HV_Archer_HighAttack.HV_Archer_HighAttack'"));
	AltAttackAnim[1] = AltAttackHighFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AltAttackLowFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Archer/HV_Archer_LowAttack.HV_Archer_LowAttack'"));
	AltAttackAnim[2] = AltAttackLowFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AltDashFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Archer/HV_Archer_Dash.HV_Archer_Dash'"));
	AltDashAnim = AltDashFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AltDeathFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Archer/HV_Archer_Death.HV_Archer_Death'"));
	AltDeathAnim = AltDeathFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AltIdleFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Archer/HV_Archer_Idle.HV_Archer_Idle'"));
	AltIdleAnim = AltIdleFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AltRunFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Archer/HV_Archer_Run.HV_Archer_Run'"));
	AltRunAnim = AltRunFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AltTakeHitFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Archer/HV_Archer_Hurt.HV_Archer_Hurt'"));
	AltHurtAnim = AltTakeHitFlipbookAsset.Object;

	static ConstructorHelpers::FObjectFinder<UPaperSprite>ArrowSpriteAsset(TEXT("PaperSprite'/Game/Textures/MyTextures/Arrow_Sprite.Arrow_Sprite'"));
	ArrowSprite = ArrowSpriteAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperSprite>AltArrowSpriteAsset(TEXT("PaperSprite'/Game/Textures/MyTextures/HV_Arrow_Sprite.HV_Arrow_Sprite'"));
	AltArrowSprite = AltArrowSpriteAsset.Object;

	//Wwise
	static ConstructorHelpers::FObjectFinder<UAkAudioEvent>AttackEventAsset(TEXT("AkAudioEvent'/Game/WwiseData/Events/Archer/Play_ArcherAttack1.Play_ArcherAttack1'"));
	AttackEvent = AttackEventAsset.Object;
	static ConstructorHelpers::FObjectFinder<UAkAudioEvent>DashEventAsset(TEXT("AkAudioEvent'/Game/WwiseData/Events/CharacterShared/Play_Dodge.Play_Dodge'"));
	DashEvent = DashEventAsset.Object;
	static ConstructorHelpers::FObjectFinder<UAkAudioEvent>LandEventAsset(TEXT("AkAudioEvent'/Game/WwiseData/Events/PlayerCharacter/Play_Land.Play_Land'"));
	LandEvent = LandEventAsset.Object;
	static ConstructorHelpers::FObjectFinder<UAkAudioEvent>FootstepEventAsset(TEXT("AkAudioEvent'/Game/WwiseData/Events/CharacterShared/Play_Footsteps.Play_Footsteps'"));
	FootstepEvent = FootstepEventAsset.Object;

	// Configure character movement
	MoveComp->GravityScale = 2.0f;
	MoveComp->GroundFriction = 3.0f;
	MoveComp->BrakingFrictionFactor = 2.f;
	MoveComp->MaxWalkSpeed = 500.0f;
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
	Health.CurrentPoints = Health.MaxPoints = 40.f;
	StaminaRegenRate = 20.f;

	bAttacking = false;
	bSaveAttack = false;
	ArrowDamage = 20.f;
	ArrowSpeed = 1500.f;
	ShootCost = 20.f;

	DashCost = 10.f;
	DashPower = 1750.f;
	DashCooldown = 0.5;
	bDashing = false;

	bDeathSequenceStarted = false;
	DeathTimer = 0.f;
}

void AEnemy_Archer::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemy_Archer::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	AudioDevice->StopGameObject(AudioComp);
}

void AEnemy_Archer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bAttacking && !MoveComp->IsFalling())
		UpdateStamina(StaminaRegenRate * DeltaTime);

	if (AmIDead() && !bDeathSequenceStarted) {
		bDeathSequenceStarted = true;
		//GetWorld()->GetGameState<ADOOMCloneGameState>()->OnEnemyKilled.Broadcast();
		Cast<AEnemyController_Archer>(GetController())->GetBlackboardComponent()->SetValueAsBool(FName("bDeathSequenceStarted"), true);

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

	if (!bAttacking && !bDashing) {
		TArray<AActor*> OverlappingActors;
		DodgeTrigger->GetOverlappingActors(OverlappingActors, APlayerCharacter::StaticClass());
		for (AActor* Actor : OverlappingActors) {
			if (Actor != this && !DashTimer.IsValid()) {
				Dash();
				break;
			}
		}
	}

	UpdateCharacter();
}

void AEnemy_Archer::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	//AudioDevice->PostEvent(LandEvent, this);
	PlaySfx(LandEvent, false);
}

void AEnemy_Archer::Attack(int32 AttackIndex)
{
	if (bIsAnimInterruptible && Stamina.CurrentPoints > ShootCost) {
		bAttacking = true;
		UpdateStamina(-ShootCost);

		FTimerHandle ShootTimer, SfxTimer;
		FTimerDelegate ShootDelegate, SfxDelegate;
		switch (AttackIndex)
		{
		case 0:
			//ShootDelegate.BindSP(this, &AEnemy_Archer::ShootArrow);
			ShootDelegate.BindUFunction(this, FName("ShootArrow"));
			GetWorldTimerManager().SetTimer(ShootTimer, ShootDelegate, 16.f / 24.f, false);
			SfxDelegate.BindUFunction(this, "PlaySfx", AttackEvent, true);
			GetWorldTimerManager().SetTimer(SfxTimer, SfxDelegate, 16.f / 24.f, false);
			break;
		case 1:
			//ShootDelegate.BindSP(this, &AEnemy_Archer::ShootArrowHigh);
			ShootDelegate.BindUFunction(this, FName("ShootArrowHigh"));
			GetWorldTimerManager().SetTimer(ShootTimer, ShootDelegate, 18.f / 24.f, false);
			SfxDelegate.BindUFunction(this, "PlaySfx", AttackEvent, true);
			GetWorldTimerManager().SetTimer(SfxTimer, SfxDelegate, 18.f / 24.f, false);
			break;
		case 2:
			//ShootDelegate.BindSP(this, &AEnemy_Archer::ShootArrowLow);
			ShootDelegate.BindUFunction(this, FName("ShootArrowLow"));
			GetWorldTimerManager().SetTimer(ShootTimer, ShootDelegate, 20.f / 24.f, false);
			SfxDelegate.BindUFunction(this, "PlaySfx", AttackEvent, true);
			GetWorldTimerManager().SetTimer(SfxTimer, SfxDelegate, 20.f / 24.f, false);
			break;
		default:
			break;
		}

		bIsAnimInterruptible = false;
		MoveComp->StopMovementImmediately();
		if (bUseAltAnims) {
			PlayOneShotAnim(AltAttackAnim[AttackIndex]);
		}
		else {
			PlayOneShotAnim(AttackAnim[AttackIndex]);
		}
	}
}

void AEnemy_Archer::ShootArrow()
{
	if (bAttacking) {
		FVector Location = GetActorLocation() + FVector(12.f, 0.f, 7.f);
		FRotator Trajectory = Controller->GetControlRotation() + FRotator(0.f, 0.f, 0.f);
		TArray<TSubclassOf<ACharacterBase>>ClassesToIgnore;
		ClassesToIgnore.Add(AEnemy_Archer::StaticClass());
		ClassesToIgnore.Add(AEnemy_EvilWizard::StaticClass());

		AProjectileBase* NewProjectile = GetWorld()->SpawnActor<AProjectileBase>(Location, Trajectory);
		NewProjectile->SetActorScale3D(FVector(5.f));
		if (bUseAltAnims) {
			NewProjectile->SetInitialParams(ArrowSpeed, 0.5f, ArrowDamage, 1, &ClassesToIgnore, AltArrowSprite);
		}
		else {
			NewProjectile->SetInitialParams(ArrowSpeed, 0.5f, ArrowDamage, 1, &ClassesToIgnore, ArrowSprite);
		}
	}
	bAttacking = false;
}

void AEnemy_Archer::ShootArrowHigh()
{
	if (bAttacking) {
		FVector Location = GetActorLocation() + FVector(11.f, 0.f, 12.5f);
		FRotator Trajectory = Controller->GetControlRotation() + FRotator(25.f, 0.f, 0.f);
		TArray<TSubclassOf<ACharacterBase>>ClassesToIgnore;
		ClassesToIgnore.Add(AEnemy_Archer::StaticClass());
		ClassesToIgnore.Add(AEnemy_EvilWizard::StaticClass());

		AProjectileBase* NewProjectile = GetWorld()->SpawnActor<AProjectileBase>(Location, Trajectory);
		NewProjectile->SetActorScale3D(FVector(5.f));
		if (bUseAltAnims) {
			NewProjectile->SetInitialParams(ArrowSpeed, 1.f, ArrowDamage, 1, &ClassesToIgnore, AltArrowSprite);
		}
		else {
			NewProjectile->SetInitialParams(ArrowSpeed, 1.f, ArrowDamage, 1, &ClassesToIgnore, ArrowSprite);
		}
	}
	bAttacking = false;
}

void AEnemy_Archer::ShootArrowLow()
{
	if (bAttacking) {
		FVector Location = GetActorLocation() + FVector(11.f, 0.f, 2.5f);
		FRotator Trajectory = Controller->GetControlRotation() + FRotator(0.f, 0.f, 0.f);
		TArray<TSubclassOf<ACharacterBase>>ClassesToIgnore;
		ClassesToIgnore.Add(AEnemy_Archer::StaticClass());
		ClassesToIgnore.Add(AEnemy_EvilWizard::StaticClass());

		AProjectileBase* NewProjectile = GetWorld()->SpawnActor<AProjectileBase>(Location, Trajectory);
		NewProjectile->SetActorScale3D(FVector(5.f));
		if (bUseAltAnims) {
			NewProjectile->SetInitialParams(ArrowSpeed, 0.5f, ArrowDamage, 1, &ClassesToIgnore, AltArrowSprite);
		}
		else {
			NewProjectile->SetInitialParams(ArrowSpeed, 0.5f, ArrowDamage, 1, &ClassesToIgnore, ArrowSprite);
		}
	}
	bAttacking = false;
}

void AEnemy_Archer::Dash()
{
	FVector DashDirection = (GetActorLocation() - UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation()).GetSafeNormal();
	bool bCanDash = (bIsAnimInterruptible && !bDashing && FMath::Abs(DashDirection.X) > 0.f && Stamina.CurrentPoints > DashCost);

	if (bCanDash) {
		UpdateStamina(-DashCost);
		bSaveAttack = false;

		MoveComp->BrakingFrictionFactor = 0.f;
		HitBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

		LaunchCharacter(FVector(DashDirection.X, 0.f, 0.f).GetSafeNormal() * DashPower, true, true);

		GetWorldTimerManager().ClearTimer(DashTimer);
		GetWorldTimerManager().SetTimer(DashTimer, this, &AEnemy_Archer::StopDash, DashAnim->GetTotalDuration(), false);
		bCanTakeDamage = false;
		GetWorldTimerManager().ClearTimer(IFrameTimer);
		GetWorldTimerManager().SetTimer(IFrameTimer, this, &AEnemy_Archer::DisableIFrames, DashAnim->GetTotalDuration(), false);

		bIsAnimInterruptible = false;
		if (bUseAltAnims) {
			PlayOneShotAnim(AltDashAnim);
		}
		else {
			PlayOneShotAnim(DashAnim);
		}
		//AudioDevice->PostEvent(DashEvent, this);
		PlaySfx(DashEvent, false);

		bDashing = true;
	}
}

void AEnemy_Archer::StopDash()
{
	const FVector Velocity = GetVelocity();
	float TravelDirection = Velocity.X;
	if (TravelDirection < 0.0f)
		Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
	else if (TravelDirection > 0.0f)
		Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));

	HitBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	MoveComp->StopMovementImmediately();
	MoveComp->BrakingFrictionFactor = 2.f;

	GetWorldTimerManager().ClearTimer(DashTimer);
	GetWorldTimerManager().SetTimer(DashTimer, this, &AEnemy_Archer::ResetDash, DashCooldown, false);
}

void AEnemy_Archer::ResetDash()
{
	GetWorldTimerManager().ClearTimer(DashTimer);
	bDashing = false;
}

void AEnemy_Archer::DisableIFrames()
{
	bCanTakeDamage = true;
}

void AEnemy_Archer::UpdateCharacter()
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

void AEnemy_Archer::UpdateAnimation()
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
				float Delay[2] = { 4.f / 24.f, 12.f / 24.f };
				for (size_t i = 0; i < 2; i++)
				{
					FTimerDelegate SfxDelegate;
					SfxDelegate.BindUFunction(this, "PlaySfx", FootstepEvent, false);
					GetWorldTimerManager().ClearTimer(FootTimer[i]);
					GetWorldTimerManager().SetTimer(FootTimer[i], SfxDelegate, 16.f / 24.f, true, Delay[i]);
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

void AEnemy_Archer::PlaySfx(UAkAudioEvent* SfxToPlay, bool bTriggerSfxOnCompass)
{
	//AudioDevice->PostEvent(SfxToPlay, this);
	AudioComp->PostAkEvent(SfxToPlay, 0, FOnAkPostEventCallback(), TArray<FAkExternalSourceInfo>(), SfxToPlay->GetFName().ToString());
	if (bTriggerSfxOnCompass)
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD<APlayerHUD>()->SfxTriggered(GetActorLocation(), 0.75f);
}

void AEnemy_Archer::TakeHit(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
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

		Cast<AEnemyController_Archer>(GetController())->GetBlackboardComponent()->SetValueAsBool(FName("bIsHurtAnimPlaying"), true);

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
