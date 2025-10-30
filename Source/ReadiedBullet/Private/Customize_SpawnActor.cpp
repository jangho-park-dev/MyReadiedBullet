// Fill out your copyright notice in the Description page of Project Settings.


#include "Customize_SpawnActor.h"

// Sets default values
ACustomize_SpawnActor::ACustomize_SpawnActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACustomize_SpawnActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACustomize_SpawnActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

