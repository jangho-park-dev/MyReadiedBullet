// Fill out your copyright notice in the Description page of Project Settings.


#include "Cube.h"
#include "Engine.h"


// Sets default values
ACube::ACube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));

	/*static ConstructorHelpers::FObjectFinder<UStaticMesh>
		STATICMESH_BODY(TEXT("/Game/Geometry/Meshes/1M_Cube"));

	if (STATICMESH_BODY.Succeeded())
	{
		StaticMesh->SetStaticMesh(STATICMESH_BODY.Object);
	}*/

}

// Called when the game starts or when spawned
void ACube::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector ForwardVector = StaticMesh->GetForwardVector();
	ForwardVector = ForwardVector * 10;
	StaticMesh->AddWorldOffset(ForwardVector);
}

