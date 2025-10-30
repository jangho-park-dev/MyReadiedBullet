// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ReadiedBullet/ReadiedBullet.h"
#include "GameFramework/Actor.h"
#include "RBGameInstance.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Sound/SoundBase.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Door.generated.h"

UCLASS()
class READIEDBULLET_API ADoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Root")
	UStaticMeshComponent* Root;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "StaticMesh")
	UStaticMeshComponent* leftDoor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "StaticMesh")
	UStaticMeshComponent* rightDoor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CapsuleColl")
	UBoxComponent* BoxCom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
	class UAudioComponent* AudioComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
	USoundBase* DoorOpenCue;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	float leftDoorAngle = -180.0f;
	float rightDoorAngel = 0.0f;

	bool openDoor = false;

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
