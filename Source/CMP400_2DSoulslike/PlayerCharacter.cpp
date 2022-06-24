// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

// Paper 2D
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"

// AkAudio - Wwise
#include "AkAudioEvent.h"
#include "AkComponent.h"

// Components
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Kismet
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"

// Perception
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AISense_Sight.h"

// My headers
#include "SoulslikePlayerController.h"

#include "AttackHitbox.h"
#include "InteractableBase.h"

#include "Hazard_Collision.h"
#include "Hazard_DoT.h"

#include "WriteToFileManager.h"

DEFINE_LOG_CATEGORY_STATIC(PlayerCharacter, Log, All);

APlayerCharacter::APlayerCharacter()
{
	// Set the size of our collision capsule.
	HitBox->SetCapsuleSize(8.f, 15.f, true);
	HitBox->SetRelativeScale3D(FVector(5.f, 5.f, 5.f));

	ListenerPosition = CreateDefaultSubobject<USceneComponent>(TEXT("ListnerComponent"));

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 750.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 75.0f);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	// Create an orthographic camera (no perspective) and attach it to the boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	SideViewCameraComponent->OrthoWidth = 2048.0f;
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	// Prevent all automatic rotation behavior on the camera, character, and camera component
	CameraBoom->SetUsingAbsoluteRotation(true);
	SideViewCameraComponent->bUsePawnControlRotation = false;
	SideViewCameraComponent->bAutoActivate = true;
	SideViewCameraComponent->SetOrthoWidth(2560.f);

	FlipbookComponent->SetRelativeLocation(FVector(0.f, 0.f, 1.5f));
	FlipbookComponent->OnFinishedPlaying.AddDynamic(this, &ACharacterBase::ResetFlipbook);

	// Damage Event
	OnTakeAnyDamage.AddDynamic(this, &APlayerCharacter::TakeHit);

	// AI perception
	PercSys = UAIPerceptionSystem::GetCurrent(GetWorld());
	if (PercSys) {
		PercSys->RegisterSenseClass(UAISense_Sight::StaticClass());
		PercSys->RegisterSourceForSenseClass(UAISense_Sight::StaticClass(), *this);
	}

	// Get all animation flipbooks
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>Attack1FlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Rogue/PlayerCharacter_Attack1.PlayerCharacter_Attack1'"));
	AttackAnim[0] = Attack1FlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>Attack2FlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Rogue/PlayerCharacter_Attack2.PlayerCharacter_Attack2'"));
	AttackAnim[1] = Attack2FlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>Attack3FlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Rogue/PlayerCharacter_Attack3.PlayerCharacter_Attack3'"));
	AttackAnim[2] = Attack3FlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AttackStopFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Rogue/PlayerCharacter_AttackStop.PlayerCharacter_AttackStop'"));
	AttackStopAnim = AttackStopFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>CastFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Rogue/PlayerCharacter_Cast.PlayerCharacter_Cast'"));
	CastAnim = CastFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>DashFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Rogue/PlayerCharacter_Dash.PlayerCharacter_Dash'"));
	DashAnim = DashFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>DeathFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Rogue/PlayerCharacter_Death.PlayerCharacter_Death'"));
	DeathAnim = DeathFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>FallFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Rogue/PlayerCharacter_Fall.PlayerCharacter_Fall'"));
	FallAnim = FallFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>IdleFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Rogue/PlayerCharacter_Idle.PlayerCharacter_Idle'"));
	IdleAnim = IdleFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>JumpFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Rogue/PlayerCharacter_Jump.PlayerCharacter_Jump'"));
	JumpAnim = JumpFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>LandFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Rogue/PlayerCharacter_Land.PlayerCharacter_Land'"));
	LandAnim = LandFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>NoBlurDashFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Rogue/PlayerCharacter_NoBlur_Dash.PlayerCharacter_NoBlur_Dash'"));
	NoBlurDashAnim = NoBlurDashFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>RunFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Rogue/PlayerCharacter_Run.PlayerCharacter_Run'"));
	RunAnim = RunFlipbookAsset.Object;

	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AltAttack1FlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Rogue/HV_PlayerCharacter_Attack1.HV_PlayerCharacter_Attack1'"));
	AltAttackAnim[0] = AltAttack1FlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AltAttack2FlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Rogue/HV_PlayerCharacter_Attack2.HV_PlayerCharacter_Attack2'"));
	AltAttackAnim[1] = AltAttack2FlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AltAttack3FlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Rogue/HV_PlayerCharacter_Attack3.HV_PlayerCharacter_Attack3'"));
	AltAttackAnim[2] = AltAttack3FlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AltAttackStopFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Rogue/HV_PlayerCharacter_AttackStop.HV_PlayerCharacter_AttackStop'"));
	AltAttackStopAnim = AltAttackStopFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AltCastFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Rogue/HV_PlayerCharacter_Cast.HV_PlayerCharacter_Cast'"));
	AltCastAnim = AltCastFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AltDashFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Rogue/HV_PlayerCharacter_Dash.HV_PlayerCharacter_Dash'"));
	AltDashAnim = AltDashFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AltDeathFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Rogue/HV_PlayerCharacter_Death.HV_PlayerCharacter_Death'"));
	AltDeathAnim = AltDeathFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AltFallFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Rogue/HV_PlayerCharacter_Fall.HV_PlayerCharacter_Fall'"));
	AltFallAnim = AltFallFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AltIdleFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Rogue/HV_PlayerCharacter_Idle.HV_PlayerCharacter_Idle'"));
	AltIdleAnim = AltIdleFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AltJumpFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Rogue/HV_PlayerCharacter_Jump.HV_PlayerCharacter_Jump'"));
	AltJumpAnim = AltJumpFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AltLandFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Rogue/HV_PlayerCharacter_Land.HV_PlayerCharacter_Land'"));
	AltLandAnim = AltLandFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AltNoBlurDashFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Rogue/HV_PlayerCharacter_NoBlur_Dash.HV_PlayerCharacter_NoBlur_Dash'"));
	AltNoBlurDashAnim = AltNoBlurDashFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AltRunFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Rogue/HV_PlayerCharacter_Run.HV_PlayerCharacter_Run'"));
	AltRunAnim = AltRunFlipbookAsset.Object;
	
	//Wwise
	static ConstructorHelpers::FObjectFinder<UAkAudioEvent>AttackEvent0Asset(TEXT("AkAudioEvent'/Game/WwiseData/Events/PlayerCharacter/Play_Attack_01.Play_Attack_01'"));
	AttackEvent[0] = AttackEvent0Asset.Object;
	static ConstructorHelpers::FObjectFinder<UAkAudioEvent>AttackEvent1Asset(TEXT("AkAudioEvent'/Game/WwiseData/Events/PlayerCharacter/Play_Attack_02.Play_Attack_02'"));
	AttackEvent[1] = AttackEvent1Asset.Object;
	static ConstructorHelpers::FObjectFinder<UAkAudioEvent>AttackEvent2Asset(TEXT("AkAudioEvent'/Game/WwiseData/Events/PlayerCharacter/Play_Attack_03.Play_Attack_03'"));
	AttackEvent[2] = AttackEvent2Asset.Object;
	static ConstructorHelpers::FObjectFinder<UAkAudioEvent>DashEventAsset(TEXT("AkAudioEvent'/Game/WwiseData/Events/CharacterShared/Play_Dodge.Play_Dodge'"));
	DashEvent = DashEventAsset.Object;
	static ConstructorHelpers::FObjectFinder<UAkAudioEvent>JumpEventAsset(TEXT("AkAudioEvent'/Game/WwiseData/Events/PlayerCharacter/Play_Jump.Play_Jump'"));
	JumpEvent = JumpEventAsset.Object;
	static ConstructorHelpers::FObjectFinder<UAkAudioEvent>LandEventAsset(TEXT("AkAudioEvent'/Game/WwiseData/Events/PlayerCharacter/Play_Land.Play_Land'"));
	LandEvent = LandEventAsset.Object;
	static ConstructorHelpers::FObjectFinder<UAkAudioEvent>FootstepEventAsset(TEXT("AkAudioEvent'/Game/WwiseData/Events/CharacterShared/Play_Footsteps.Play_Footsteps'"));
	FootstepEvent = FootstepEventAsset.Object;

	// Configure character movement
	MoveComp->GravityScale = 2.0f;
	MoveComp->AirControl = 1.f;
	MoveComp->JumpZVelocity = 1000.f;
	MoveComp->GroundFriction = 3.5f;
	MoveComp->BrakingFrictionFactor = 2.f;
	MoveComp->MaxWalkSpeed = 1000.0f;
	//MoveComp->MaxFlySpeed = 600.0f;
	JumpMaxCount = 2;
	JumpMaxHoldTime = 0.f;

	//StatusEffectComp = CreateDefaultSubobject<UStatusEffectComponent>("Status Effect Comp");

	// Enable replication on the Sprite component so animations show up when networked
	GetSprite()->SetIsReplicated(true);
	bReplicates = true;

	// Set default animation state
	AnimState = EAnimState::EAS_Idle;

	// Defaults
	StaminaRegenRate = 33.3f;

	bCanDashInAir = false;
	DashCost = 10;
	DashPower = 2000;
	DashCooldown = 0.1;
	bDashing = false;

	bAttacking = false;
	bSaveAttack = false;
	MeleeDamage = 20;
	MeleeCost = 20;
	AttackCount = 0;

	JumpCount = 0;

	InteractableObject = nullptr;

	bCanTakeDamage = true;

	bDeathSequenceEnded = bDeathSequenceStarted = false;
	DeathTimer = 0.f;

	NewQueueItemTimer = 0.f;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	AudioComp->IsDefaultListener = true;
	AudioDevice->AddDefaultListener(AudioComp);
	AudioDevice->SetSpatialAudioListener(AudioComp);
	//PlayerController->SetAudioListenerOverride(ListenerPosition, FVector(0.f), FRotator(0.f));
	
	PositionQueue.Add(GetActorLocation());

	// Hitbox
	MeleeHitbox = GetWorld()->SpawnActor<AAttackHitbox>();
	MeleeHitbox->SetupHitbox(FVector(36.f, 32.f, 64.f), this, MeleeDamage);
	MeleeHitbox->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	MeleeHitbox->SetActorRelativeLocation(FVector(15.f, 0.f, 0.f));
	MeleeHitbox->SetActorRelativeScale3D(FVector(0.2f));
}

void APlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	AudioComp->IsDefaultListener = false;
	AudioDevice->RemoveDefaultListener(AudioComp);
	//PlayerController->ClearAudioListenerOverride();

	// Position Queue
	FString Msg = FDateTime::Now().ToString() + ',' + UGameplayStatics::GetCurrentLevelName(GetWorld()) + ',' + LastDamageCauser;
	for (FVector Pos : PositionQueue)
		Msg += (",X: " + FString::SanitizeFloat(Pos.X) + " Y: " + FString::SanitizeFloat(Pos.Z));

	for (size_t i = 0; i < 5 - PositionQueue.Num(); i++)
		Msg += ",X: NaN Y: NaN";

	// Death Position
	FVector DeathPos = GetActorLocation();
	Msg += (",X: " + FString::SanitizeFloat(DeathPos.X) + " Y: " + FString::SanitizeFloat(DeathPos.Z));

	//WriteToFileManager::AppendLineToFile(Msg, UKismetSystemLibrary::GetProjectSavedDirectory(), "DeathPositionQueue.csv");

	AudioDevice->StopGameObject(AudioComp);
	MeleeHitbox->Destroy();
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	NewQueueItemTimer += DeltaTime;
	if (NewQueueItemTimer >= 1.f) {
		PositionQueue.Add(GetActorLocation());
		if (PositionQueue.Num() > 5)
			PositionQueue.RemoveAt(0);

		NewQueueItemTimer = 0.f;
	}

	if (!bAttacking && !bDashing && !MoveComp->IsFalling())
		UpdateStamina(StaminaRegenRate * DeltaTime);

	if (MoveComp->IsFalling() && JumpCount == 0) {
		bCanDashInAir = true;
		JumpCount = 1;
	}

	if (AmIDead() && !bDeathSequenceStarted) {
		bDeathSequenceStarted = true;

		FlipbookComponent->Stop();

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
		bDeathSequenceEnded = true;

	UpdateCharacter();
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	PlayerController = Cast<ASoulslikePlayerController>(NewController);
}

void APlayerCharacter::MoveRight(float Value)
{
	if (!bAttacking && !bDeathSequenceStarted) {
		// Apply the input to the character motion
		AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
	}
}

void APlayerCharacter::Jump()
{
	bool bCanJump = (bIsAnimInterruptible && JumpCount < 2 && !PlayerController->IsMenuOpen() && !bDeathSequenceStarted);
	if (bCanJump) {
		Super::Jump();
		bCanDashInAir = true;
		bSaveAttack = false;

		bCanTakeDamage = false;
		GetWorldTimerManager().ClearTimer(IFrameTimer);
		GetWorldTimerManager().SetTimer(IFrameTimer, this, &APlayerCharacter::DisableIFrames, JumpAnim->GetTotalDuration(), false);

		if (!MoveComp->IsFalling() || JumpCount < 2) {
			bIsAnimInterruptible = false;
			if (bUseAltAnims) {
				PlayOneShotAnim(AltJumpAnim);
			}
			else {
				PlayOneShotAnim(JumpAnim);
			}
			PlaySfx(JumpEvent);
		}
		JumpCount++;
	}
}

void APlayerCharacter::StopJumping()
{
	Super::StopJumping();
}

void APlayerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	UpdateStamina(-5.f);

	if (bIsAnimInterruptible) {
		bIsAnimInterruptible = false;
		if (bUseAltAnims) {
			PlayOneShotAnim(AltLandAnim);
		}
		else {
			PlayOneShotAnim(LandAnim);
		}
		PlaySfx(LandEvent);
	}

	JumpCount = 0;
}

void APlayerCharacter::Dash()
{
	FVector DashDirection = MoveComp->GetLastInputVector();
	bool bCanDash = (bIsAnimInterruptible && !bDashing && FMath::Abs(DashDirection.X) > 0.f && Stamina.CurrentPoints > DashCost && !PlayerController->IsMenuOpen());
	bool bGroundDash = (bCanDash && !MoveComp->IsFalling());
	bool bAirDash = (bCanDash && bCanDashInAir && MoveComp->IsFalling());
	
	if ((bGroundDash || bAirDash) && !bDeathSequenceStarted) {
		UpdateStamina(-DashCost);
		bCanDashInAir = false;
		bSaveAttack = false;

		MoveComp->BrakingFrictionFactor = 0.f;
		HitBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

		LaunchCharacter(FVector(DashDirection.X, 0.f, 0.f).GetSafeNormal() * DashPower, true, true);
		
		GetWorldTimerManager().ClearTimer(DashTimer);
		GetWorldTimerManager().SetTimer(DashTimer, this, &APlayerCharacter::StopDash, DashAnim->GetTotalDuration(), false);
		bCanTakeDamage = false;
		GetWorldTimerManager().ClearTimer(IFrameTimer);
		GetWorldTimerManager().SetTimer(IFrameTimer, this, &APlayerCharacter::DisableIFrames, DashAnim->GetTotalDuration(), false);

		bIsAnimInterruptible = false;
		if (bUseAltAnims) {
			PlayOneShotAnim(AltDashAnim);
		}
		else {
			PlayOneShotAnim(DashAnim);
		}
		PlaySfx(DashEvent);

		bDashing = true;
	}
}

void APlayerCharacter::StopDash()
{
	HitBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	FVector FallVelocity = MoveComp->GetLastUpdateVelocity();
	MoveComp->StopMovementImmediately();
	MoveComp->BrakingFrictionFactor = 2.f;
	MoveComp->Velocity = FVector(0.f, 0.f, FallVelocity.Z);

	GetWorldTimerManager().ClearTimer(DashTimer);
	GetWorldTimerManager().SetTimer(DashTimer, this, &APlayerCharacter::ResetDash, DashCooldown, false);
}

void APlayerCharacter::ResetDash()
{
	GetWorldTimerManager().ClearTimer(DashTimer);
	bDashing = false;
}

void APlayerCharacter::MeleeAttack()
{
	if (!PlayerController->IsMenuOpen()) {
		if (bAttacking) {
			bSaveAttack = true;
			return;
		}
		else {
			AttackTriggered();
		}
	}
}

void APlayerCharacter::AttackTriggered()
{
	if (bIsAnimInterruptible && Stamina.CurrentPoints > MeleeCost && !bDeathSequenceStarted) {
		UpdateStamina(-MeleeCost);

		GetWorldTimerManager().ClearTimer(AttackTimer);
		GetWorldTimerManager().SetTimer(AttackTimer, this, &APlayerCharacter::ComboAttack, AttackAnim[AttackCount]->GetTotalDuration() - 0.1f, false);
		

		FTimerHandle AttackHitboxTimer;
		FTimerDelegate DamageDelegate;
		DamageDelegate.BindUFunction(this, "DamageEnemies");
		switch (AttackCount) {
		case 0:
			MeleeHitbox->SetActorRelativeLocation(FVector(15.f, 0.f, 0.f));
			MeleeHitbox->ChangeShape(FVector(36.f, 32.f, 64.f));
			GetWorldTimerManager().SetTimer(AttackHitboxTimer, DamageDelegate, 9.f / 24.f, false);
			break;
		case 1:
			MeleeHitbox->SetActorRelativeLocation(FVector(14.f, 0.f, 0.f));
			MeleeHitbox->ChangeShape(FVector(30.f, 32.f, 64.f));
			GetWorldTimerManager().SetTimer(AttackHitboxTimer, DamageDelegate, 4.f / 24.f, false);
			break;
		case 2:
			MeleeHitbox->SetActorRelativeLocation(FVector(16.f, 0.f, 0.f));
			MeleeHitbox->ChangeShape(FVector(42.f, 32.f, 64.f));
			GetWorldTimerManager().SetTimer(AttackHitboxTimer, DamageDelegate, 5.f / 24.f, false);
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

		if (AttackCount > 2)
			AttackCount = 0;

		bSaveAttack = false;
		bAttacking = true;
	}
	else {
		ResetCombo();
	}
}

void APlayerCharacter::DamageEnemies()
{
	MeleeHitbox->DamageEnemies();
}

void APlayerCharacter::ComboAttack()
{
	bIsAnimInterruptible = true;
	if (bSaveAttack) {
		AttackTriggered();
	}
	else {
		ResetCombo();
	}
}

void APlayerCharacter::ResetCombo()
{
	if (bAttacking) {
		bIsAnimInterruptible = false;
		if (bUseAltAnims) {
			PlayOneShotAnim(AltAttackStopAnim);
		}
		else {
			PlayOneShotAnim(AttackStopAnim);
		}
	}

	AttackCount = 0;
	bSaveAttack = false;
	bAttacking = false;
}

void APlayerCharacter::DisableIFrames()
{
	bCanTakeDamage = true;
}

void APlayerCharacter::Interact()
{
	if (InteractableObject)
		InteractableObject->OnInteract.Broadcast();
}

void APlayerCharacter::SetHotbarSlot(int32 SlotIndex)
{
	// Do nothing for now
}

void APlayerCharacter::UpdateCharacter()
{
	// Update animation to match the motion
	UpdateAnimation();

	// Now setup the rotation of the controller based on the direction we are travelling
	const FVector PlayerVelocity = GetVelocity();
	float TravelDirection = PlayerVelocity.X;
	// Set the rotation so that the character faces his direction of travel.
	if (PlayerController != nullptr)
	{
		if (TravelDirection < 0.0f)
		{
			PlayerController->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
		}
		else if (TravelDirection > 0.0f)
		{
			PlayerController->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
		}
	}

	if (FMath::IsNearlyZero(PlayerVelocity.X) || MoveComp->IsFalling()) {
		GetWorldTimerManager().ClearTimer(FootTimer[0]);
		GetWorldTimerManager().ClearTimer(FootTimer[1]);
	}
}

void APlayerCharacter::UpdateAnimation()
{
	const FVector PlayerVelocity = GetVelocity();

	EAnimState AirMovement = (FMath::Abs(PlayerVelocity.Z) > 0.0f) ? EAnimState::EAS_Falling : EAnimState::EAS_Idle;
	EAnimState GroundMovement = MoveComp->IsFalling() ? AirMovement : EAnimState::EAS_Running;
	EAnimState NotMoving = (PlayerVelocity.Size() != 0) ? GroundMovement : EAnimState::EAS_Idle;
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
					SfxDelegate.BindUFunction(this, "PlaySfx", FootstepEvent);
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
		case EAnimState::EAS_Falling:
			if (bUseAltAnims) {
				FlipbookComponent->SetFlipbook(AltFallAnim);
			}
			else {
				FlipbookComponent->SetFlipbook(FallAnim);
			}
			break;
		default:
			break;
		}

		if (!FlipbookComponent->IsPlaying())
			FlipbookComponent->Play();
	}
}

void APlayerCharacter::PlaySfx(UAkAudioEvent* SfxToPlay)
{
	//AudioDevice->PostEvent(SfxToPlay, this);
	AudioComp->PostAkEvent(SfxToPlay, 0, FOnAkPostEventCallback(), TArray<FAkExternalSourceInfo>(), SfxToPlay->GetFName().ToString());
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	// Movement
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &APlayerCharacter::Dash);

	//PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerCharacter::Crouch);
	//PlayerInputComponent->BindAction("Crouch", IE_Released, this, &APlayerCharacter::Uncrouch);
	//PlayerInputComponent->BindAction("Toggle Crouch", IE_Pressed, this, &APlayerCharacter::ToggleCrouch);

	// Interact
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::Interact);
	//PlayerInputComponent->BindAction("Use Item", IE_Pressed, this, &APlayerCharacter::UseItem);

	// Combat
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::MeleeAttack);

	//UI
	//PlayerInputComponent->BindAction("Next Hotbar Item", IE_Pressed, this, &APlayerCharacter::NextHotbarItem);
	//PlayerInputComponent->BindAction("Prev Hotbar Item", IE_Pressed, this, &APlayerCharacter::PrevHotbarItem);

	// Ugly & Bad I'm sorry
	PlayerInputComponent->BindAction<FQuickHotbarDelegate>("HotbarSlot1", IE_Pressed, this, &APlayerCharacter::SetHotbarSlot, 0);
	PlayerInputComponent->BindAction<FQuickHotbarDelegate>("HotbarSlot2", IE_Pressed, this, &APlayerCharacter::SetHotbarSlot, 1);
	PlayerInputComponent->BindAction<FQuickHotbarDelegate>("HotbarSlot3", IE_Pressed, this, &APlayerCharacter::SetHotbarSlot, 2);
	PlayerInputComponent->BindAction<FQuickHotbarDelegate>("HotbarSlot4", IE_Pressed, this, &APlayerCharacter::SetHotbarSlot, 3);
	PlayerInputComponent->BindAction<FQuickHotbarDelegate>("HotbarSlot5", IE_Pressed, this, &APlayerCharacter::SetHotbarSlot, 4);
	PlayerInputComponent->BindAction<FQuickHotbarDelegate>("HotbarSlot6", IE_Pressed, this, &APlayerCharacter::SetHotbarSlot, 5);
	PlayerInputComponent->BindAction<FQuickHotbarDelegate>("HotbarSlot7", IE_Pressed, this, &APlayerCharacter::SetHotbarSlot, 6);
	PlayerInputComponent->BindAction<FQuickHotbarDelegate>("HotbarSlot8", IE_Pressed, this, &APlayerCharacter::SetHotbarSlot, 7);
	PlayerInputComponent->BindAction<FQuickHotbarDelegate>("HotbarSlot9", IE_Pressed, this, &APlayerCharacter::SetHotbarSlot, 8);
	PlayerInputComponent->BindAction<FQuickHotbarDelegate>("HotbarSlot0", IE_Pressed, this, &APlayerCharacter::SetHotbarSlot, 9);
}

void APlayerCharacter::SetInteractable(AInteractableBase* Interactable)
{
	InteractableObject = Interactable;
}

void APlayerCharacter::TakeHit(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (bCanTakeDamage) {
		UpdateStamina(-(Damage / 2.f));

		//bIsAnimInterruptible = false;
		//FlipbookComponent->SetLooping(false);

		//FlipbookComponent->SetFlipbook(HurtAnim);
		//FlipbookComponent->Play();

		//if (DamageType->GetClass()->ImplementsInterface(UDamageTypeInterface::StaticClass())) {
		//	IDamageTypeInterface* DamageInterface = (IDamageTypeInterface*)Cast<IDamageTypeInterface>(DamageType);
		//	if (DamageInterface)
		//		DamageInterface->TriggerEffect(StatusEffectComp);
		//}

		if (DamageCauser->GetClass() == AHazard_Collision::StaticClass())
			MoveComp->AddImpulse((GetActorLocation() - DamageCauser->GetActorLocation()).GetSafeNormal() * 500.f, true);
		else if(DamageCauser->GetClass() == AHazard_DoT::StaticClass())
			MoveComp->AddImpulse((GetActorLocation() - DamageCauser->GetActorLocation()).GetSafeNormal() * 100.f, true);

		// check damage
		if (Damage < 0.f)
			return;

		//bPlayTakeHealthHitAnim = true;
		UpdateHealth(-Damage);

		LastDamageCauser = DamageCauser->GetName();
	}
}
