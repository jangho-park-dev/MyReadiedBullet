// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "MonsterWidget.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UENUM(BlueprintType)
enum class enemyCharacterName : uint8
{
	Barghest UMETA(DisplayName = "Barghest"),
	Dragon UMETA(DisplayName = "Dragon"),
	Centaur UMETA(DisplayName = "Centaur"),
	Griffon UMETA(DisplayName = "Griffon"),
};

UCLASS()
class READIEDBULLET_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();
	float GetFinalAttackRange() const;
	float GetFinalAttackRange_Dragon() const;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stat)
	enemyCharacterName eName;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	UWidgetComponent* HPBarWidget;
};