// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyItem.h"

// Sets default values
AKeyItem::AKeyItem()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;
	CapCom = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleColl"));
	CapCom->SetupAttachment(StaticMesh);
	CapCom->ComponentTags.Add(FName("Key"));
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AKeyItem::BeginPlay()
{
	Super::BeginPlay();
	CapCom->OnComponentBeginOverlap.AddDynamic(this, &AKeyItem::BeginOverlap);
	
}

// Called every frame
void AKeyItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CapCom->AddLocalRotation(FRotator(0.0f, 5.0f, 0.0f));
}

void AKeyItem::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	this->Destroy();
}


