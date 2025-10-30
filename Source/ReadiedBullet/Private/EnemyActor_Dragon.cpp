
#include "EnemyActor_Dragon.h"
#include "RBGameInstance.h"

// Sets default values
AEnemyActor_Dragon::AEnemyActor_Dragon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CAPSULE"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MESH"));
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBarWidget"));

	RootComponent = Capsule;
	Mesh->SetupAttachment(Capsule);
	HPBarWidget->SetupAttachment(Mesh);

	Capsule->SetCapsuleHalfHeight(150.f);
	Capsule->SetCapsuleRadius(100.f);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> DRAGON_BODY(TEXT("/Game/QuadrapedCreatures/MountainDragon/Meshes/SK_MOUNTAIN_DRAGON.SK_MOUNTAIN_DRAGON"));
	if (DRAGON_BODY.Succeeded())
	{
		Mesh->SetSkeletalMesh(DRAGON_BODY.Object);
	}

	Mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> DRAGON_ANIM(TEXT("/Game/QuadrapedCreatures/Blueprints/DragonAnimBlueprint.DragonAnimBlueprint_C"));
	if (DRAGON_ANIM.Succeeded())
	{
		Mesh->SetAnimInstanceClass(DRAGON_ANIM.Class);
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/UI/WBP_HPBar"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(200.0f, 80.0));
	}

	static ConstructorHelpers::FObjectFinder<USoundBase>FIRESOUND(TEXT("SoundWave'/Game/Sound/dragonBreath.dragonBreath'"));
	if (FIRESOUND.Succeeded())
	{
		FireCue = FIRESOUND.Object;
	}

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio_Fire"));
	AudioComponent->bAutoActivate = false;
	AudioComponent->SetupAttachment(Mesh);

	//애니메이션 재생을 위한 애니메이션 바인딩
	static ConstructorHelpers::FObjectFinder<UAnimSequence> IdleAnim(TEXT("AnimSequence'/Game/QuadrapedCreatures/MountainDragon/Animations/ANIM_MOUNTAIN_DRAGON_FlyStationary.ANIM_MOUNTAIN_DRAGON_FlyStationary'"));
	static ConstructorHelpers::FObjectFinder<UAnimSequence> FireAnim(TEXT("AnimSequence'/Game/QuadrapedCreatures/MountainDragon/Animations/ANIM_MOUNTAIN_DRAGON_FlyStationarySpitFireBall.ANIM_MOUNTAIN_DRAGON_FlyStationarySpitFireBall'"));
	S_IdleAnim = IdleAnim.Object;
	S_FireAnim = FireAnim.Object;

	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 500.0f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);

	MaxHP = 1.0f;
	IsDead = false;
	BulletSpread = 2.0f;
	fireTime = 0.0f;
}

// Called when the game starts or when spawned
void AEnemyActor_Dragon::BeginPlay()
{
	Super::BeginPlay();

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AEnemyActor_Dragon::BeginOverlap);
	Capsule->OnComponentBeginOverlap.AddDynamic(this, &AEnemyActor_Dragon::BeginOverlap);

	Mesh->PlayAnimation(S_IdleAnim, true);
}


// Called every frame
void AEnemyActor_Dragon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	URBGameInstance* GameInstance = Cast<URBGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));


	if (bIsAttack) {
		fireTime += DeltaTime;
		if (!fireAnimActive) {
			Mesh->PlayAnimation(S_FireAnim, true);
			fireAnimActive = true;
		}

		if (fireTime > 1.6f)
		{
			Fire();
			fireTime = 0.0f;
		}
	}
}


void AEnemyActor_Dragon::BeginOverlap(UPrimitiveComponent* OverlappedComponent,
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
		SetDeadAnim();
		this->Destroy();
	}
}

void AEnemyActor_Dragon::Fire()
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
		meshLoc.Z = meshLoc.Z + 1200;

		// spawn the projectile at the muzzle
		GetWorld()->SpawnActor<AActor>(ProjectileClass, meshLoc, Mesh->GetComponentRotation(), ActorSpawnParams);
	}

	//LastFireTime = GetWorld()->TimeSeconds;

}

void AEnemyActor_Dragon::AnimNotify_FireEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("ax"));
}

void AEnemyActor_Dragon::SetDeadAnim()
{
	URBGameInstance* GameInstance = Cast<URBGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	IsDead = true;
	UE_LOG(LogTemp, Warning, TEXT("ax"));
	GameInstance->Boss1Die = true;
	
}