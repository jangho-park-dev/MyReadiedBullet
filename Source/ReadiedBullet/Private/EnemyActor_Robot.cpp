// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyActor_Robot.h"

// Sets default values
AEnemyActor_Robot::AEnemyActor_Robot()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RC = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	Head = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshHead"));
	Body = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshBody"));
	SideBody = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshSideBody"));
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBarWidget"));

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio_Fire"));
	AudioComponent->bAutoActivate = false;
	AudioComponent->SetupAttachment(Body);


	static ConstructorHelpers::FObjectFinder<USoundBase>FIRESOUND(TEXT("SoundWave'/Game/Sound/Tank_Fire.Tank_Fire'"));
	if (FIRESOUND.Succeeded())
	{
		FireCue = FIRESOUND.Object;
	}


	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/UI/WBP_HPBar"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(200.0f, 80.0));
	}
	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);


	RootComponent = RC;
	Head->SetupAttachment(Body);
	SideBody->SetupAttachment(Body);
	HPBarWidget->SetupAttachment(Body);
	fireTime = 0.0f;
	missileTime = 0.0f;
	bMove = true;
	MaxHP = 1.0f;
	dir = 0;
	isAttack = false;
}

// Called when the game starts or when spawned
void AEnemyActor_Robot::BeginPlay()
{
	Super::BeginPlay();

	Body->OnComponentBeginOverlap.AddDynamic(this, &AEnemyActor_Robot::BeginOverlap);
	Head->OnComponentBeginOverlap.AddDynamic(this, &AEnemyActor_Robot::BeginOverlap);
}

// Called every frame
void AEnemyActor_Robot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (isAttack) {

		FVector playerLoc = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
		FRotator rot = UKismetMathLibrary::FindLookAtRotation(Head->GetComponentLocation(), playerLoc);
		Head->SetWorldRotation(rot);

		fireTime += DeltaTime;
		missileTime += DeltaTime;

		if (fireTime > 1.6f)
		{
			Fire();
			fireTime = 0.0f;
		}
		if (missileTime > 2.0f)
		{
			Fire2();
			missileTime = 0.0f;
		}


		if (bMove)
		{
			if (dir == 0)
			{
				AddActorWorldOffset(FVector(-10.0f, 0.0f, 0.0f));
				if (this->GetActorLocation().X < 6000)
				{
					bMove = false;
					dir = 1;
				}
			}
			else if (dir == 1)
			{
				AddActorWorldOffset(FVector(0.0f, -10.0f, 0.0f));
				if (this->GetActorLocation().Y < -10000)
				{
					bMove = false;
					dir = 2;
				}
			}
			else if (dir == 2)
			{
				AddActorWorldOffset(FVector(10.0f, 0.0f, 0.0f));
				if (this->GetActorLocation().X > 12000)
				{
					bMove = false;
					dir = 3;
				}
			}
			else if (dir == 3)
			{
				AddActorWorldOffset(FVector(0.0f, 10.0f, 0.0f));
				if (this->GetActorLocation().Y > 10000)
				{
					bMove = false;
					dir = 0;
				}
			}
		}


		if (!bMove)
		{
			AddActorLocalRotation(FRotator(0, 1.0f, 0));

			if (GetActorRotation().Yaw > 89.0f && GetActorRotation().Yaw < 91.0f)
			{
				SetActorRotation(FRotator(0.0f, 91.1f, 0.0f));
				bMove = true;
			}
			else if (GetActorRotation().Yaw > 179.0f && GetActorRotation().Yaw < 181.0f)
			{
				SetActorRotation(FRotator(0.0f, 181.1f, 0.0f));
				bMove = true;
			}
			else if (GetActorRotation().Yaw < -89.0f && GetActorRotation().Yaw > -91.0f)
			{
				SetActorRotation(FRotator(0.0f, -88.9f, 0.0f));
				bMove = true;
			}
			else if (GetActorRotation().Yaw > -0.5f && GetActorRotation().Yaw < 0.5f)
			{
				SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
				bMove = true;
			}
		}
	}
}

void AEnemyActor_Robot::Fire()
{
	AudioComponent->AttenuationSettings;
	AudioComponent->SetSound(FireCue);
	AudioComponent->Play();

	FVector EyeLocation;
	FRotator EyeRotation;

	this->GetActorEyesViewPoint(EyeLocation, EyeRotation);


	FVector ShotDirection = EyeRotation.Vector();

	// Bullet Spread
	float HalfRad = FMath::DegreesToRadians(BulletSpread);
	ShotDirection = FMath::VRandCone(ShotDirection, HalfRad, HalfRad);

	FVector TraceEnd = EyeLocation + (ShotDirection * 10000);
	if (ProjectileClass)
	{
		//무기 위치 받아서 저장
		FVector MuzzleLocation = Head->GetSocketLocation("Gun_EndSocket");
		//FRotator MuzzleRotation = MeshComp->GetSocketRotation("MuzzleFlashSocket");;

		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		FVector meshLoc = Head->GetComponentLocation();
		meshLoc.Z = meshLoc.Z + 150;
		// spawn the projectile at the muzzle
		GetWorld()->SpawnActor<AActor>(ProjectileClass, meshLoc, Head->GetComponentRotation(), ActorSpawnParams);
	}

	//LastFireTime = GetWorld()->TimeSeconds;

}

void AEnemyActor_Robot::Fire2()
{
	//미사일 사운드는 HomingMissile.cpp에서 재생

	FVector EyeLocation;
	FRotator EyeRotation;

	this->GetActorEyesViewPoint(EyeLocation, EyeRotation);


	FVector ShotDirection = EyeRotation.Vector();

	// Bullet Spread
	float HalfRad = FMath::DegreesToRadians(BulletSpread);
	ShotDirection = FMath::VRandCone(ShotDirection, HalfRad, HalfRad);

	FVector TraceEnd = EyeLocation + (ShotDirection * 10000);
	if (MissileClass)
	{
		//무기 위치 받아서 저장
		FVector MuzzleLocation = SideBody->GetSocketLocation("Gun_EndSocket");
		//FRotator MuzzleRotation = MeshComp->GetSocketRotation("MuzzleFlashSocket");;

		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		FVector meshLoc = SideBody->GetComponentLocation();
		// spawn the projectile at the muzzle
		GetWorld()->SpawnActor<AActor>(MissileClass, meshLoc, SideBody->GetComponentRotation(), ActorSpawnParams);
		//GetWorld()->SpawnActor<AActor>(MissileClass, meshLoc, FRotator(60.0f, 0.0f, 0.0f), ActorSpawnParams);
	}
	//LastFireTime = GetWorld()->TimeSeconds;
}


void AEnemyActor_Robot::BeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	UMonsterWidget* MW = Cast<UMonsterWidget>(HPBarWidget->GetUserWidgetObject());
	MaxHP -= 0.02f;
	MW->HPProgressBar->SetPercent(MaxHP);

	if (MaxHP <= 0.001f)
	{
		//여기다가 뒤지는 애니메이션 해주셈
		this->Destroy();
	}
}