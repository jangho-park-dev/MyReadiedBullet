// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ReadiedBullet.h"
#include "GameFramework/Actor.h"
#include "Projectile.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "DrawDebugHelpers.h"
#include "RBWeapon.generated.h"

UCLASS()
class READIEDBULLET_API ARBWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARBWeapon();

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
    TSubclassOf<AActor> PathClass;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void PlayFireEffects(FVector TraceEnd);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USkeletalMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		TSubclassOf<UDamageType> DamageType;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		FName MuzzleSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		UParticleSystem* DefaultImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		UParticleSystem* FleshImpactEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		FName TracerTargetName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		UParticleSystem* TracerEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		TSubclassOf<UCameraShake> FireCamShake;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		float BaseDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
		class UAudioComponent* AudioComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
	USoundBase* FireCue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
    USoundBase* ReloadCue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
    USoundBase* FireMissCue;

	FTimerHandle TimerHandle_TimeBetweenShots;

	float LastFireTime;

	/*ºÐ´ç ÃÑ¾Ë ½î´Â ¼Óµµ RPM*/
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float RateOfFire;

	//RateOfFireÀ» ³ª´²ÁØ´Ù.
	float TimeBetweenShots;


public:
	void Fire();
	virtual void StartFire();

	void StopFire();
	void Reload();

	bool IsReloading = false;
	int32 Magazine;
	int32 CurrentArmo;
	
	float ReloadCount = 0;
};