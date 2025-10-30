// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RBGameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Projectile.generated.h"

#define FIRE_IMPACT 10
#define THUNDER_IMPACT 11
#define ICE_IMPACT 12
#define LASER_IMPACT 13
#define DIR_REVERSE 14

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOverlapParam);


UCLASS()
class READIEDBULLET_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;

	

	/** called when projectile hits something */
	//UFUNCTION()
	//void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 SelectBulletSlot;

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

	/** Sphere collision component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	USceneComponent* SceneComp;

	



	UPROPERTY(EditDefaultsOnly, Category = "Bullet")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Slot")
	TArray<FTransform>  BoxTransform1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Slot")
	TArray<FTransform>  CylinderTransform1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Slot")
	TArray<FTransform>  SphereTransform1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Slot")
	TArray<FTransform>  BoxTransform2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Slot")
	TArray<FTransform>  CylinderTransform2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Slot")
	TArray<FTransform>  SphereTransform2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Slot")
	TArray<FTransform>  BoxTransform3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Slot")
	TArray<FTransform>  CylinderTransform3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Slot")
	TArray<FTransform>  SphereTransform3;



	/*UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> CubeBlueprint;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> CylinderBlueprint;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> SphereBlueprint;*/

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float RotateX1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float RotateY1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float RotateZ1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector RotateVector1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float RotateX2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float RotateY2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float RotateZ2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector RotateVector2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float RotateX3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float RotateY3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float RotateZ3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector RotateVector3;



	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<UStaticMeshComponent*> BoxActorArray1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<UStaticMeshComponent*> CylinderActorArray1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<UStaticMeshComponent*> SphereActorArray1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<UStaticMeshComponent*> BoxActorArray2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<UStaticMeshComponent*> CylinderActorArray2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<UStaticMeshComponent*> SphereActorArray2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<UStaticMeshComponent*> BoxActorArray3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<UStaticMeshComponent*> CylinderActorArray3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<UStaticMeshComponent*> SphereActorArray3;



	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Instance")
	int32 BulletImpactSolt1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Instance")
	int32 BulletImpactSolt2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Instance")
	int32 BulletImpactSolt3;
	

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TEST")
	TArray<UStaticMeshComponent*> BoxStaticMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
    UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintAssignable, BlueprintCallable)
    FOverlapParam OverlapParam;

	//Q?§Î? ?ÑÎ•¥Î©?Ï¥ùÏïå Î∞©Ìñ• ??†Ñ???ÑÌïú Î≥Ä??
	float BulletTempY1;
	float BulletTempZ1;
	float BulletTempY2;
	float BulletTempZ2;
	float BulletTempY3;
	float BulletTempZ3;
};
