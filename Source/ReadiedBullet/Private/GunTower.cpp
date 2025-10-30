// Fill out your copyright notice in the Description page of Project Settings.


#include "GunTower.h"

// Sets default values
AGunTower::AGunTower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SKMesh"));
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBarWidget"));
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio_Fire"));

	AudioComponent->bAutoActivate = false;
	AudioComponent->SetupAttachment(Mesh);

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

	static ConstructorHelpers::FObjectFinder<UAnimSequence> FireAnim(TEXT("AnimSequence'/Game/CSC/Animations/Anim_CSC_Gun4.Anim_CSC_Gun4'"));
	S_FireAnim = FireAnim.Object;

	MaxHP = 1.0f;
	isFire = false;
	fireTime = 0.0f;
	towerAngleY = 0.0f;
}

// Called when the game starts or when spawned
void AGunTower::BeginPlay()
{
	Super::BeginPlay();
	HPBarWidget->SetVisibility(false);
}

// Called every frame
void AGunTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	/*FVector Loc = GetActorLocation();
	FVector PlayerLoc = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
	float dis = UKismetMathLibrary::Vector_Distance(PlayerLoc, Loc);
	if (dis < 3500)
	{
		fireTime += DeltaTime;
		if (Mesh->GetComponentRotation().Roll > 5.0f) {
			Mesh->AddWorldRotation(FRotator(-5.0f, 0.0f, 0.0f));
			
		}
		else
		{
			isFire = true;
		}

	}
	else
	{
		fireTime = 0.0f;
		if (Mesh->GetComponentRotation().Roll < 176.0f) {
			Mesh->AddWorldRotation(FRotator(5.0f, 0.0f, 0.0f));
			isFire = false;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("%f"), Mesh->GetComponentRotation().Roll);
	if (isFire)
	{
		FVector playerLoc = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
		FRotator rot = UKismetMathLibrary::FindLookAtRotation(Mesh->GetComponentLocation(), playerLoc);
		Mesh->SetWorldRotation(rot);
	}

	if (isFire && fireTime > 2.5f) {
		Mesh->PlayAnimation(S_FireAnim, false);
		Fire();
		HPBarWidget->SetVisibility(true);
		fireTime = 0.0f;
	}*/
}

void AGunTower::Fire()
{

	AudioComponent->AttenuationSettings;
	AudioComponent->SetSound(FireCue);
	AudioComponent->Play(1.0f);

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
		FVector MuzzleLocation = Mesh->GetSocketLocation("FireSocket");
		//FRotator MuzzleRotation = MeshComp->GetSocketRotation("MuzzleFlashSocket");;

		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		FVector meshLoc = Mesh->GetComponentLocation();

		// spawn the projectile at the muzzle
		GetWorld()->SpawnActor<AActor>(ProjectileClass, meshLoc, Mesh->GetComponentRotation(), ActorSpawnParams);
	}

	//LastFireTime = GetWorld()->TimeSeconds;

}