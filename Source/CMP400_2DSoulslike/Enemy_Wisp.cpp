// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Wisp.h"

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
#include "EnemyController_Wisp.h"
#include "PlayerCharacter.h"
#include "PlayerHUD.h"

#include "AttackHitbox.h"

AEnemy_Wisp::AEnemy_Wisp()
{
	// Set the size of our collision capsule.
	HitBox->SetCapsuleSize(4.f, 8.f, true);
	HitBox->SetRelativeScale3D(FVector(5.f, 5.f, 5.f));
	HitBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	HitBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemy_Wisp::OnOverlapBegin);

	FlipbookComponent->SetRelativeLocation(FVector(0.f, 0.f, 2.5f));
	FlipbookComponent->SetRelativeScale3D(FVector(0.75f));
	FlipbookComponent->OnFinishedPlaying.AddDynamic(this, &AEnemy_Wisp::ResetFlipbook);

	// Damage Event
	OnTakeAnyDamage.AddDynamic(this, &AEnemy_Wisp::TakeHit);

	// AI
	AutoPossessPlayer = EAutoReceiveInput::Disabled;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AEnemyController_Wisp::StaticClass();

	// Get all animation flipbooks
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>DeathFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Executioner/Summon/Summon_Death.Summon_Death'"));
	DeathAnim = DeathFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>RunFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Executioner/Summon/Summon_Idle.Summon_Idle'"));
	RunAnim = RunFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>SummonFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Executioner/Summon/Summon_Spawn.Summon_Spawn'"));
	SummonAnim = SummonFlipbookAsset.Object;

	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AltDeathFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Executioner/Summon/HV_Summon_Death.HV_Summon_Death'"));
	AltDeathAnim = AltDeathFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AltRunFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Executioner/Summon/HV_Summon_Idle.HV_Summon_Idle'"));
	AltRunAnim = AltRunFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AltSummonFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Kronovi_Executioner/Summon/HV_Summon_Spawn.HV_Summon_Spawn'"));
	AltSummonAnim = AltSummonFlipbookAsset.Object;

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
	Health.CurrentPoints = Health.MaxPoints = 1.f;

	MeleeDamage = 5.f;

	bDeathSequenceStarted = false;
	DeathTimer = 0.f;
}

void AEnemy_Wisp::BeginPlay()
{
	Super::BeginPlay();

	bIsAnimInterruptible = false;
	Cast<AEnemyController_Wisp>(GetController())->GetBlackboardComponent()->SetValueAsBool(FName("bSpawnAnimFinished"), false);
	if (bUseAltAnims) {
		PlayOneShotAnim(AltSummonAnim);
	}
	else {
		PlayOneShotAnim(SummonAnim);
	}

	FTimerHandle LifeTimer;
	GetWorldTimerManager().SetTimer(LifeTimer, this, &AEnemy_Wisp::KillSelf, 20.f, false);
}

void AEnemy_Wisp::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	AudioDevice->StopGameObject(AudioComp);
}

void AEnemy_Wisp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AmIDead() && !bDeathSequenceStarted) {
		bDeathSequenceStarted = true;
		Cast<AEnemyController_Wisp>(GetController())->GetBlackboardComponent()->SetValueAsBool(FName("bDeathSequenceStarted"), true);

		bIsAnimInterruptible = false;
		if (bUseAltAnims) {
			PlayOneShotAnim(AltDeathAnim);
		}
		else {
			PlayOneShotAnim(DeathAnim);
		}
	}

	if (bDeathSequenceStarted && DeathTimer < DeathAnim->GetTotalDuration()) {
		DeathTimer += DeltaTime;
	}
	else if (bDeathSequenceStarted && DeathTimer >= DeathAnim->GetTotalDuration()) {
		Destroy();
	}

	UpdateCharacter();
}

void AEnemy_Wisp::KillSelf()
{
	UGameplayStatics::ApplyDamage(this, MeleeDamage, nullptr, this, TSubclassOf<UDamageType>());
}

void AEnemy_Wisp::UpdateCharacter()
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

void AEnemy_Wisp::UpdateAnimation()
{
	AnimState = EAnimState::EAS_Running;

	if (bIsAnimInterruptible) {
		if (bUseAltAnims) {
			FlipbookComponent->SetFlipbook(AltRunAnim);
		}
		else {
			FlipbookComponent->SetFlipbook(RunAnim);
		}

		if (!FlipbookComponent->IsPlaying())
			FlipbookComponent->Play();
	}
}

void AEnemy_Wisp::PlaySfx(UAkAudioEvent* SfxToPlay, bool bTriggerSfxOnCompass)
{
	//AudioDevice->PostEvent(SfxToPlay, this);
	AudioComp->PostAkEvent(SfxToPlay, 0, FOnAkPostEventCallback(), SfxToPlay->GetFName().ToString());
	if (bTriggerSfxOnCompass)
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD<APlayerHUD>()->SfxTriggered(GetActorLocation(), 0.75f);
}

void AEnemy_Wisp::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if (Player) {
		UGameplayStatics::ApplyDamage(Player, MeleeDamage, nullptr, this, TSubclassOf<UDamageType>());
		UGameplayStatics::ApplyDamage(this, MeleeDamage, nullptr, this, TSubclassOf<UDamageType>());
	}
}

void AEnemy_Wisp::TakeHit(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (!bDeathSequenceStarted) {
		MoveComp->AddImpulse((GetActorLocation() - DamageCauser->GetActorLocation()).GetSafeNormal() * 500.f, true);

		// check damage
		if (Damage < 0.f)
			return;

		UpdateHealth(-Damage);

		//if (GEngine)
		//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Boss hit, health = %f"), Health.CurrentPoints));
	}
}
