// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "CusPawn.h"
#include "Cube.generated.h"

UCLASS()
class READIEDBULLET_API ACube : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACube();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "StaticMesh")
	UStaticMeshComponent* StaticMesh;


	virtual void Tick(float DeltaTime) override;

};
