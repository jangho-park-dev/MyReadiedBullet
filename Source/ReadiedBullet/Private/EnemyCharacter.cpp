// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "BarghestAIController.h"
#include "EnemyAnimInstance.h"
#include <string>

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add(TEXT("Alive"));
	
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBarWidget"));
	HPBarWidget->SetupAttachment(GetMesh());

	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 500.0f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);

	MaxHP = 1.0f;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);

	AIControllerClass = ABarghestAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;


	FString fs(this->GetPathName());
	std::string mst(TCHAR_TO_UTF8(*fs));
	
	if (mst.find("Wolf") != -1)
	{
		eName = enemyCharacterName::Barghest;
	}
	else if (mst.find("Dragon") != -1)
	{
		eName = enemyCharacterName::Dragon;
	}
	else if (mst.find("Centaur") != -1)
	{
		eName = enemyCharacterName::Centaur;
	}
	else if (mst.find("Griffon") != -1)
	{
		eName = enemyCharacterName::Griffon;
	}
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::BeginOverlap);
}


// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AEnemyCharacter::BeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	UMonsterWidget* MW = Cast<UMonsterWidget>(HPBarWidget->GetUserWidgetObject());
	MaxHP -= 0.2f;
	MW->HPProgressBar->SetPercent(MaxHP);
	auto casted_animinstance = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());

	casted_animinstance->setState(CharacterAnimState::HIT);

	if (MaxHP <= 0.01f)
	{
		casted_animinstance->setState(CharacterAnimState::DEAD);
	}
}

float AEnemyCharacter::GetFinalAttackRange() const
{
	return 150.f;
}

float AEnemyCharacter::GetFinalAttackRange_Dragon() const
{
	return 350.f;
}