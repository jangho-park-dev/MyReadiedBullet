// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Animation/AnimSequence.h"
#include "Animation/AnimInstance.h"
#include "MonsterWidget.h"
#include "Components/AudioComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundBase.h"
#include "Sound/SoundAttenuation.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "GameFramework/Actor.h"
#include "EnemyActor_Dragon.generated.h"


UCLASS()
class READIEDBULLET_API AEnemyActor_Dragon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyActor_Dragon();
	void SetDeadAnim();

	void Fire();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stat)
	float MaxHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stat)
	float CurrentHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Collision)
	UCapsuleComponent* Capsule;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Visual)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	UWidgetComponent* HPBarWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = DRAGON, Meta = (AllowPrivateAccess = true))
	bool IsDead;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (ClampMin = 0.0f))
	float BulletSpread;

	UAnimSequence* S_FireAnim;
	UAnimSequence* S_IdleAnim;

	float fireTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Attack)
	bool bIsAttack = false;

	bool fireAnimActive = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
		USoundBase* FireCue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
		class UAudioComponent* AudioComponent;

private:

	UFUNCTION()
	void AnimNotify_FireEnd();
};