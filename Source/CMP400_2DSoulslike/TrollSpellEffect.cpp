// Fill out your copyright notice in the Description page of Project Settings.


#include "TrollSpellEffect.h"

// Paper 2D
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"

// AkAudio - Wwise
#include "AkAudioEvent.h"
#include "AkComponent.h"

// Components
#include "Components/BoxComponent.h"

// Helpers
#include "Kismet/GameplayStatics.h"

// My headers
#include "PlayerCharacter.h"
#include "PlayerHUD.h"

ATrollSpellEffect::ATrollSpellEffect()
{
	PrimaryActorTick.bCanEverTick = true;

	AudioComp = CreateDefaultSubobject<UAkComponent>(TEXT("Audio Comp"));
	AudioDevice = FAkAudioDevice::Get();

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent->SetRelativeScale3D(FVector(5.f, 5.f, 5.f));

	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("FlipbookComp"));
	FlipbookComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	FlipbookComponent->SetRelativeLocation(FVector(3.f, 0.f, 0.f));
	FlipbookComponent->SetRelativeScale3D(FVector(0.75f));
	FlipbookComponent->OnFinishedPlaying.AddDynamic(this, &ATrollSpellEffect::Kill);

	Hitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hitbox"));
	Hitbox->InitBoxExtent(FVector(70.f, 32.f, 145.f));
	Hitbox->SetRelativeScale3D(FVector(0.2f));
	Hitbox->SetRelativeLocation(FVector(-7.f, 0.f, -17.f));
	Hitbox->SetGenerateOverlapEvents(true);
	Hitbox->SetCollisionResponseToChannels(ECollisionResponse::ECR_Overlap);
	Hitbox->AttachToComponent(FlipbookComponent, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>SpellAnimFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Clembod_BringerOfDeath/BoD_Spell.BoD_Spell'"));
	SpellAnim = SpellAnimFlipbookAsset.Object;
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>AltSpellAnimFlipbookAsset(TEXT("PaperFlipbook'/Game/Anims/Clembod_BringerOfDeath/HV_BoD_Spell.HV_BoD_Spell'"));
	AltSpellAnim = AltSpellAnimFlipbookAsset.Object;

	static ConstructorHelpers::FObjectFinder<UAkAudioEvent>StrikeEventAsset(TEXT("AkAudioEvent'/Game/WwiseData/Events/Troll/Play_TrollCastImpact.Play_TrollCastImpact'"));
	StrikeEvent = StrikeEventAsset.Object;

	SpellDamage = 10;
	bIsStrikeOn = false;
}

void ATrollSpellEffect::BeginPlay()
{
	Super::BeginPlay();
}

void ATrollSpellEffect::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	AudioDevice->StopGameObject(AudioComp);
}

void ATrollSpellEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsStrikeOn) {

		TArray<AActor*> OverlappingActors;
		Hitbox->GetOverlappingActors(OverlappingActors, APlayerCharacter::StaticClass());
		for (AActor* Actor : OverlappingActors) {
			UGameplayStatics::ApplyDamage(Actor, SpellDamage, nullptr, this, UDamageType::StaticClass());
			TurnOffStrike();
		}
	}
}

void ATrollSpellEffect::TurnOnStrike()
{
	bIsStrikeOn = true;
	AudioComp->PostAkEvent(StrikeEvent, 0, FOnAkPostEventCallback(), TArray<FAkExternalSourceInfo>(), StrikeEvent->GetFName().ToString());
	//UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD<APlayerHUD>()->SfxTriggered(GetActorLocation(), 1.5f);
}

void ATrollSpellEffect::TurnOffStrike()
{
	bIsStrikeOn = false;
}

void ATrollSpellEffect::Kill()
{
	Destroy();
}

void ATrollSpellEffect::InitializeSpell(float InSpellDamage, bool UseAltAnims)
{
	SpellDamage = InSpellDamage;

	FlipbookComponent->SetLooping(false);
	if (UseAltAnims) {
		FlipbookComponent->SetFlipbook(AltSpellAnim);
	}
	else {
		FlipbookComponent->SetFlipbook(SpellAnim);
	}
	FlipbookComponent->Play();

	FTimerHandle StrikeOnTimer;
	FTimerDelegate StrikeOnDelegate;
	StrikeOnDelegate.BindUFunction(this, "TurnOnStrike");
	GetWorldTimerManager().SetTimer(StrikeOnTimer, StrikeOnDelegate, 14.f / 24.f, false);

	FTimerHandle StrikeOffTimer;
	FTimerDelegate StrikeOffDelegate;
	StrikeOffDelegate.BindUFunction(this, "TurnOffStrike");
	GetWorldTimerManager().SetTimer(StrikeOffTimer, StrikeOffDelegate, 26.f / 24.f, false);
}
