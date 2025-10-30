// Fill out your copyright notice in the Description page of Project Settings.

#include "BulletComp_BOX.h"
#include "Engine.h"

// Sets default values
ABulletComp_BOX::ABulletComp_BOX()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Box"));
	LeftBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left"));
	RightBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right"));
	UpBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Up"));
	DownBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Down"));
	FrontBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Front"));
	BackBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Back"));

	RootComponent = BoxComp;
	LeftBox->SetupAttachment(BoxComp);
	RightBox->SetupAttachment(BoxComp);
	UpBox->SetupAttachment(BoxComp);
	DownBox->SetupAttachment(BoxComp);
	FrontBox->SetupAttachment(BoxComp);
	BackBox->SetupAttachment(BoxComp);

	//델리게이트 함수 연결
	LeftBox->OnComponentBeginOverlap.AddDynamic(this, &ABulletComp_BOX::OnOverlapBegin_LeftBox);
	LeftBox->OnComponentEndOverlap.AddDynamic(this, &ABulletComp_BOX::OnOverlapEnd_LeftBox);

	RightBox->OnComponentBeginOverlap.AddDynamic(this, &ABulletComp_BOX::OnOverlapBegin_RightBox);
	RightBox->OnComponentEndOverlap.AddDynamic(this, &ABulletComp_BOX::OnOverlapEnd_RightBox);

	UpBox->OnComponentBeginOverlap.AddDynamic(this, &ABulletComp_BOX::OnOverlapBegin_UpBox);
	UpBox->OnComponentEndOverlap.AddDynamic(this, &ABulletComp_BOX::OnOverlapEnd_UpBox);

	DownBox->OnComponentBeginOverlap.AddDynamic(this, &ABulletComp_BOX::OnOverlapBegin_DownBox);
	DownBox->OnComponentEndOverlap.AddDynamic(this, &ABulletComp_BOX::OnOverlapEnd_DownBox);

	FrontBox->OnComponentBeginOverlap.AddDynamic(this, &ABulletComp_BOX::OnOverlapBegin_FrontBox);
	FrontBox->OnComponentEndOverlap.AddDynamic(this, &ABulletComp_BOX::OnOverlapEnd_FrontBox);

	BackBox->OnComponentBeginOverlap.AddDynamic(this, &ABulletComp_BOX::OnOverlapBegin_BackBox);
	BackBox->OnComponentEndOverlap.AddDynamic(this, &ABulletComp_BOX::OnOverlapEnd_BackBox);

	BoxComp->OnClicked.AddDynamic(this, &ABulletComp_BOX::BoxCompOnClicked);
	BoxComp->OnReleased.AddDynamic(this, &ABulletComp_BOX::BoxCompOnReleased);


	static ConstructorHelpers::FObjectFinder<UParticleSystem> PARTICLE_OVERLAP(TEXT("/Game/Effects/InfinityBladeEffects/Effects/FX_Skill_Leap/P_Skill_Leap_Base_Charge_Weapon"));
	if (PARTICLE_OVERLAP.Succeeded())
	{
		OverlapParticle = PARTICLE_OVERLAP.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase>FIREMISSSOUND(TEXT("SoundWave'/Game/Sound/Cus.Cus'"));
	if (FIREMISSSOUND.Succeeded())
	{
		LinkCue = FIREMISSSOUND.Object;
	}


	// 오디오 컴포넌트 추가
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("PlayerAudio"));
	AudioComponent->bAutoActivate = false;
	AudioComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABulletComp_BOX::BeginPlay()
{
	Super::BeginPlay();

	bOnCursor = false;

	LeftBox->SetCollisionObjectType(ECC_GameTraceChannel1);
	RightBox->SetCollisionObjectType(ECC_GameTraceChannel1);
	UpBox->SetCollisionObjectType(ECC_GameTraceChannel1);
	DownBox->SetCollisionObjectType(ECC_GameTraceChannel1);
	FrontBox->SetCollisionObjectType(ECC_GameTraceChannel1);
	BackBox->SetCollisionObjectType(ECC_GameTraceChannel1);

	BoxComp->SetCollisionObjectType(ECC_WorldStatic);

}

//큐브 매시에 마우스 클릭했을떄 호출되는 함수
void ABulletComp_BOX::BoxCompOnClicked(class UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	bOnCursor = true;

	ACusPawn* CusPawn = Cast<ACusPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (CusPawn != nullptr)
	{
		SpringArmRotator = CusPawn->GetSpringArmRotator();
		UE_LOG(LogTemp, Log, TEXT("Roll : %f  Pitch : %f  Yaw : %f"), SpringArmRotator.Roll, SpringArmRotator.Pitch, SpringArmRotator.Yaw);
	}
}
//큐브 매시에 마우스 클릭 땟을때 호출되는 함수
void ABulletComp_BOX::BoxCompOnReleased(class UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	bOnCursor = false;

	if (bOverlap == true)
	{
		LinkSoundPlay();
		if (bOverlapLeftBox == true)
		{

		}
		else if (bOverlapRightBox == true)
		{

		}
		else if (bOverlapUpBox == true)
		{

		}
		else if (bOverlapDownBox == true)
		{

		}
		else if (bOverlapFrontBox == true)
		{

		}
		else if (bOverlapBackBox == true)
		{

		}
		BoxComp->SetWorldLocation(WorldLocation, false);
	}
	UE_LOG(LogTemp, Log, TEXT("%s %s %s"), *FString::SanitizeFloat(WorldLocation.X), *FString::SanitizeFloat(WorldLocation.Y), *FString::SanitizeFloat(WorldLocation.Z));
}

//왼쪽 노란네모 박스에 컴포넌트가 겹쳣을때 호출되는 함수
void ABulletComp_BOX::OnOverlapBegin_LeftBox(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bOverlap = true;
	bOverlapLeftBox = true;

	WorldLocation = OtherActor->GetRootComponent()->GetChildComponent(1)->GetComponentLocation();

	UGameplayStatics::SpawnEmitterAttached(OverlapParticle, LeftBox);

	UE_LOG(LogTemp, Log, TEXT("%s %s %s"), *FString::SanitizeFloat(WorldLocation.X), *FString::SanitizeFloat(WorldLocation.Y), *FString::SanitizeFloat(WorldLocation.Z));
}

//왼쪽 노란네모 박스에서 컴포넌트 겹침이 풀릴때 호출되는 함수
void ABulletComp_BOX::OnOverlapEnd_LeftBox(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bOverlap = false;
	bOverlapLeftBox = false;
}


void ABulletComp_BOX::OnOverlapBegin_RightBox(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bOverlap = true;
	bOverlapRightBox = true;
	UGameplayStatics::SpawnEmitterAttached(OverlapParticle, RightBox);
	WorldLocation = OtherActor->GetRootComponent()->GetChildComponent(0)->GetComponentLocation();
}

void ABulletComp_BOX::OnOverlapEnd_RightBox(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bOverlap = false;
	bOverlapRightBox = false;
}

void ABulletComp_BOX::OnOverlapBegin_UpBox(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bOverlap = true;
	bOverlapUpBox = true;
	UGameplayStatics::SpawnEmitterAttached(OverlapParticle, UpBox);
	WorldLocation = OtherActor->GetRootComponent()->GetChildComponent(3)->GetComponentLocation();
}

void ABulletComp_BOX::OnOverlapEnd_UpBox(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bOverlap = false;
	bOverlapUpBox = false;
}

void ABulletComp_BOX::OnOverlapBegin_DownBox(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bOverlap = true;
	bOverlapDownBox = true;
	UGameplayStatics::SpawnEmitterAttached(OverlapParticle, DownBox);
	WorldLocation = OtherActor->GetRootComponent()->GetChildComponent(2)->GetComponentLocation();
}

void ABulletComp_BOX::OnOverlapEnd_DownBox(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bOverlap = false;
	bOverlapDownBox = false;
}

void ABulletComp_BOX::OnOverlapBegin_FrontBox(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bOverlap = true;
	bOverlapFrontBox = true;
	UGameplayStatics::SpawnEmitterAttached(OverlapParticle, FrontBox);
	WorldLocation = OtherActor->GetRootComponent()->GetChildComponent(5)->GetComponentLocation();
}

void ABulletComp_BOX::OnOverlapEnd_FrontBox(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bOverlap = false;
	bOverlapFrontBox = false;
}

void ABulletComp_BOX::OnOverlapBegin_BackBox(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bOverlap = true;
	bOverlapBackBox = true;
	UGameplayStatics::SpawnEmitterAttached(OverlapParticle, BackBox);
	WorldLocation = OtherActor->GetRootComponent()->GetChildComponent(4)->GetComponentLocation();
}

void ABulletComp_BOX::OnOverlapEnd_BackBox(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bOverlap = false;
	bOverlapBackBox = false;
}


// Called every frame
void ABulletComp_BOX::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bOnCursor == true)
	{

		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(BoxComp, 0);

		PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);

		FVector CurrentLocation = BoxComp->GetComponentLocation();
		CurrentLocation = NewWorldLocation;


		//카메라 각도에 따른 마우스 커서와 액터 위치 대입 값 방식 변경
		//위에서 아래를 볼때 or 아래에서 위를 볼때
		if ( (SpringArmRotator.Pitch == 270 || SpringArmRotator.Pitch == 90) )
		{
			NewWorldLocation.X = HitResult.Location.X;
			NewWorldLocation.Y = HitResult.Location.Y;
			NewWorldLocation.Z = CurrentLocation.Z;
		}
		//정면에서 앞을 볼때 혹은 정면에서 뒤를 볼때
		else if ((SpringArmRotator.Pitch == 360 || SpringArmRotator.Pitch == 180 || SpringArmRotator.Pitch == 0) &&
				 (SpringArmRotator.Yaw == 180 || SpringArmRotator.Yaw == 360 || SpringArmRotator.Yaw == 0))
		{
			NewWorldLocation.X = CurrentLocation.X;
			NewWorldLocation.Y = HitResult.Location.Y;
			NewWorldLocation.Z = HitResult.Location.Z;
		}
		//엑터를 옆에서 볼때
		else if ((SpringArmRotator.Pitch == 360 || SpringArmRotator.Pitch == 180 || SpringArmRotator.Pitch == 0) &&
			(SpringArmRotator.Yaw == 270 || SpringArmRotator.Yaw == 90))
		{
			NewWorldLocation.X = HitResult.Location.X;
			NewWorldLocation.Y = CurrentLocation.Y;
			NewWorldLocation.Z = HitResult.Location.Z;
		}


		BoxComp->SetWorldLocation(NewWorldLocation, false);
	}
}


void ABulletComp_BOX::LinkSoundPlay()
{
	AudioComponent->SetSound(LinkCue);
	AudioComponent->Play();
}
