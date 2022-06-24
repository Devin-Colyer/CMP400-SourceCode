// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"


#include "Kismet/GameplayStatics.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"

//#include "AkGameplayStatics.h"
//#include "AkAudioEvent.h"
//#include "AkComponent.h"

#include "PlayerCharacter.h"
#include "CharacterBase.h"
#include "InteractableBase.h"
#include "AttackHitbox.h"
#include "BossDoor.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;

	//AudioDevice = FAkAudioDevice::Get();

	RootComponent = ProjectileSprite = GetRenderComponent();
	ProjectileSprite->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	ProjectileSprite->Mobility = EComponentMobility::Movable;

	static ConstructorHelpers::FObjectFinder<UPaperSprite>ProjectileSpriteAsset(TEXT("PaperSprite'/Game/Textures/MyTextures/Arrow_Sprite.Arrow_Sprite'"));
	DefaultSprite = ProjectileSpriteAsset.Object;

	/*AKAudioComp = CreateDefaultSubobject<UAkComponent>(TEXT("AkComp"));
	AKAudioComp->SetupAttachment(ProjectileParticle);*/

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));

	MovementComponent->bConstrainToPlane = true;
	MovementComponent->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));

	MovementComponent->MaxSpeed = 10000.f;
	MovementComponent->ProjectileGravityScale = 0.05;

	DamageValue = 0;
	MaxPenetrableActors = 1;
	NumActorsPenetrated = 0;
}

void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetActorRotation(GetVelocity().Rotation());
	PositionLastFrame = GetActorLocation();
	NextPosition = PositionLastFrame + (GetVelocity() * DeltaTime);

	TraceFlightPath();
}

void AProjectileBase::TraceFlightPath()
{
	TArray<FHitResult> HitOut;

	bool bTraceSuccess = GetWorld()->LineTraceMultiByChannel(HitOut, PositionLastFrame, NextPosition, ECollisionChannel::ECC_WorldDynamic);

	if (bTraceSuccess) {
		for (FHitResult Hit : HitOut) {
			ACharacterBase* Character = Cast<ACharacterBase>(Hit.Actor);
			if (Character && !ClassesToIgnore.Contains(Character->GetClass())) {
				if (!ActorsPenetrated.Contains(Character)) {
					ActorsPenetrated.Add(Character);
					NumActorsPenetrated++;

					UGameplayStatics::ApplyDamage(Character, DamageValue, nullptr, this, UDamageType::StaticClass());
					//if (ImpactCharacterParticle != nullptr)
					//	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactCharacterParticle, Hit.ImpactPoint);

					//if (ImpactCharacterEvent != nullptr)
					//	AudioDevice->PostEventAtLocation(ImpactCharacterEvent, Hit.ImpactPoint, Hit.ImpactPoint.Rotation(), GetWorld());
				}
			}
			else if (!Cast<ACharacterBase>(Hit.Actor) && !Cast<AProjectileBase>(Hit.Actor)
				&& !Cast<AInteractableBase>(Hit.Actor) && !Cast<AVolume>(Hit.Actor)
				&& !Cast<AAttackHitbox>(Hit.Actor) && !Cast<ABossDoor>(Hit.Actor)) {
				//if (ImpactWorldParticle != nullptr)
				//	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactWorldParticle, Hit.ImpactPoint);

				//if (ImpactCharacterEvent != nullptr)
				//	AudioDevice->PostEventAtLocation(ImpactCharacterEvent, Hit.ImpactPoint, Hit.ImpactPoint.Rotation(), GetWorld());

				Destroy();
			}

			if (NumActorsPenetrated >= MaxPenetrableActors)
				Destroy();
		}
	}
}

void AProjectileBase::SetInitialParams(float Speed, float GravityScale, int32 Damage, int32 NumPenetrableActors,
	TArray<TSubclassOf<class ACharacterBase>>* InClassesToIgnore,
	UPaperSprite* Projectile)
{
	MovementComponent->MaxSpeed = Speed;
	MovementComponent->Velocity = GetActorForwardVector() * Speed;

	MovementComponent->ProjectileGravityScale = GravityScale;

	DamageValue = Damage;
	MaxPenetrableActors = NumPenetrableActors;

	if (InClassesToIgnore)
		ClassesToIgnore = *InClassesToIgnore;

	if (Projectile) {
		ProjectileSprite->SetSprite(Projectile);
	}
	else {
		ProjectileSprite->SetSprite(DefaultSprite);
	}
}