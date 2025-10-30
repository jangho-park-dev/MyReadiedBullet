// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ReadiedBullet/RBCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SceneComponent.h"
#include "Particles/ParticleSystem.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "Sound/SoundAttenuation.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "HomingMissile.generated.h"

UCLASS()
class READIEDBULLET_API AHomingMissile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHomingMissile();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collision")
	UBoxComponent* BoxColl;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "StaticMesh")
	UStaticMeshComponent* Head;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "StaticMesh")
	UStaticMeshComponent* Body;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "StaticMesh")
	UProjectileMovementComponent* ProjectileMove;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
	USoundBase* MissileCue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
	class UAudioComponent* AudioComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Timer")
	float homingTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Particle")
	UParticleSystem* Particle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

};
