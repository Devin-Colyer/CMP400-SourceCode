// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PaperSpriteActor.h"
#include "ProjectileBase.generated.h"

UCLASS()
class CMP400_2DSOULSLIKE_API AProjectileBase : public APaperSpriteActor
{
	GENERATED_BODY()

protected:
	// component pointers
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UPaperSpriteComponent* ProjectileSprite;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UPaperSprite* DefaultSprite;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UProjectileMovementComponent* MovementComponent;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//	class UAkComponent* AKAudioComp;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Feedback)
	//	UParticleSystem* ImpactCharacterParticle;
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Feedback)
	//	UParticleSystem* ImpactWorldParticle;

	/*class FAkAudioDevice* AudioDevice;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Feedback)
		UAkAudioEvent* ImpactCharacterEvent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Feedback)
		UAkAudioEvent* ImpactWorldEvent;*/

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 DamageValue;

	int32 NumActorsPenetrated;
	TArray<AActor*>ActorsPenetrated;
	TArray<TSubclassOf<class ACharacterBase>>ClassesToIgnore;

	FVector NextPosition;
	FVector PositionLastFrame;

public:
	// Sets default values for this actor's properties
	AProjectileBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ballistics)
		int32 MaxPenetrableActors;

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void TraceFlightPath();

public:
	virtual void SetInitialParams(float Speed, float GravityScale = 0.05f, int32 Damage = 0, int32 NumPenetrableActors = 1,
		TArray<TSubclassOf<class ACharacterBase>>* InClassesToIgnore = NULL,
		class UPaperSprite* Projectile = NULL);
	
};
