// Fill out your copyright notice in the Description page of Project Settings.


#include "RBCharacter.h"
#include "RBNetwork.h"
#include "RBAnimInstance.h"

// Sets default values
ARBCharacter::ARBCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add(FName("Alive"));
	Tags.Add(FName("Player"));
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->bUsePawnControlRotation = true;

	MinimapSpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("MinimapSpringArmComp"));
	MinimapCaptureComp = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Minimap"));

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("PlayerAudio"));
	AudioComponent->bAutoActivate = false;
	AudioComponent->SetupAttachment(RootComponent);
	
	SpotLightComp = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	


	static ConstructorHelpers::FObjectFinder<UAnimMontage> RELOAD(TEXT("AnimMontage'/Game/Animation/Reload_Montage.Reload_Montage'"));
	if (RELOAD.Succeeded())
	{
		ReloadMontage = RELOAD.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase>FIRESOUND(TEXT("SoundWave'/Game/Sound/Hit.Hit'"));
	if (FIRESOUND.Succeeded())
	{
		HitCue = FIRESOUND.Object;
	}

	//앉기 활성화
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	

	SpringArmComp->SetupAttachment(RootComponent);
	CameraComp->SetupAttachment(SpringArmComp);
	MinimapSpringArmComp->SetupAttachment(RootComponent);
	MinimapCaptureComp->SetupAttachment(MinimapSpringArmComp);
	SpotLightComp->SetupAttachment(CameraComp);

	ZoomedFOV = 40.0f;
	ZoomInterpSpeed = 20;

	
	WeaponAttachSocketName = "WeaponSocket";

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("RBCharacter"));

	SpotLightComp->SetAttenuationRadius(4820);
	SpotLightComp->SetInnerConeAngle(11.2f);
	SpotLightComp->SetOuterConeAngle(20.0f);
	SpotLightComp->SetLightColor(FLinearColor::Yellow);

	//////////////////Path 작업///////////////////////////////
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PATH(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	for (int32 i = 0; i < 100; i++)
	{
		FName name = *FString::Printf(TEXT("Path %i"), i);
		PathMeshArray.Add(CreateDefaultSubobject<UStaticMeshComponent>(name));
		if (PATH.Succeeded())
		{
			PathMeshArray[i]->SetStaticMesh(PATH.Object);
			PathMeshArray[i]->SetRelativeScale3D(FVector(1.2, 1.2, 1.2));
			//PathMeshArray[i]->AttachToComponent(CameraComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale); //0816
			PathMeshArray[i]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
	////////////////////////////////

	GetMesh()->ComponentTags.Add(FName("Player"));
	GetCapsuleComponent()->ComponentTags.Add(FName("Player"));

	GetCharacterMovement()->bOrientRotationToMovement = true;

	IsDead = false;
}

// Called when the game starts or when spawned
void ARBCharacter::BeginPlay()
{
	Super::BeginPlay();
	///0816
	for (int i = 0; i < 100; ++i)
	{
		PathMeshArray[i]->AttachToComponent(CameraComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
	///
	DefaultFOV = CameraComp->FieldOfView;
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ARBCharacter::BeginOverlap);
	GetMesh()->OnComponentBeginOverlap.AddDynamic(this, &ARBCharacter::BeginOverlap);
	//Spawn a Default Weapon
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	CurrentWeapon = GetWorld()->SpawnActor<ARBWeapon>(StarterWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (CurrentWeapon)
	{
		CurrentWeapon->SetOwner(this);
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);
	}

	URBGameInstance* GameInstance = Cast<URBGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	MaxHP = GameInstance->PlayerMaxHP[m_ID];

	/*TArray<AActor*> arr;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARBNetwork::StaticClass(), arr);
	m_NetWorkActor = Cast<ARBNetwork>(arr[0]);
	if (m_NetWorkActor->Get_Mode() == 1)
	{
		this->m_ID = m_NetWorkActor->m_ID;
		m_NetWorkActor->SetMyCharacter(this);
		
	}*/
}
// Called every frame
void ARBCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float TargetFOV = bWantsToZoom ? ZoomedFOV : DefaultFOV;
	float NewFOV = FMath::FInterpTo(CameraComp->FieldOfView, TargetFOV, DeltaTime, ZoomInterpSpeed);

	CameraComp->SetFieldOfView(NewFOV);

	if (IsReloading == true)
	{
		ReloadCount += DeltaTime;
		//UE_LOG(LogTemp, Warning, TEXT("%f"), ReloadCount);
		if (ReloadCount >= 3.0f)
		{
			IsReloading = false;
			ReloadCount = 0.0f;
		}
	}


	////Path 작업
	
	//////////////////////
}

void ARBCharacter::MoveForward(float Value)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);
	
	// 서버에선 이 값을 받지 않는다. 결국 이 정보는 보내지지 않는다.
	// 이 정보를 보낼 방법을 찾아야 한다.
	//UE_LOG(LogTemp, Error, TEXT("MoveF : %f, %f, %f"), this->GetActorLocation().X, this->GetActorLocation().Y, this->GetActorLocation().Z);
	//AddMovementInput(GetActorForwardVector() * Value);
}

void ARBCharacter::MoveRight(float Value)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
	//AddMovementInput(GetActorRightVector() * Value);
}

void ARBCharacter::BeginCrouch()
{
	//앉기 
	Crouch();
}

void ARBCharacter::EndCrouch()
{
	//앉기 풀기
	UnCrouch();
}

void ARBCharacter::BeginZoom()
{
	bWantsToZoom = true;
}

void ARBCharacter::EndZoom()
{
	bWantsToZoom = false;
}

void ARBCharacter::SelectSlot1()
{
	URBGameInstance* GameInstance = Cast<URBGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	GameInstance->SelectSlot[m_ID] = 1;

	if (GameInstance->IsPathMade1 == true)
	{
		for (int i = 0; i < 100; ++i)
		{
			//UE_LOG(LogTemp, Warning, TEXT("%f,  %f,  %f"), GameInstance->PathArray1[i].X, GameInstance->PathArray1[i].Y, GameInstance->PathArray1[i].Z);
			//상수는 총의 위치를 맞추기 위한 값
			PathMeshArray[i]->SetRelativeLocation(FVector(GameInstance->PathArray1[i].X + 230, GameInstance->PathArray1[i].Y - 13.0f, GameInstance->PathArray1[i].Z - 46.0f));
		}
		GameInstance->IsPathMade1 = false;
	}

	if (CurrentProjectile)
	{
		CurrentProjectile->SelectBulletSlot = 1;
		
		TArray<AActor*> network;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARBNetwork::StaticClass(), network);
		int32 size{};
		network.Empty(size);
		if (network.Num() > 0)
		{
			auto nt = Cast<ARBNetwork>(network[0]);
			if (nt)
			{
				if (nt->Get_Mode())
				{
					nt->SendBulletType((e_bulletType)GameInstance->SelectSlot[m_ID]);
				}
			}
		}
	}
}

void ARBCharacter::SelectSlot2()
{
	URBGameInstance* GameInstance = Cast<URBGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	GameInstance->SelectSlot[m_ID] = 2;

	if (GameInstance->IsPathMade2 == true)
	{
		for (int i = 0; i < 100; ++i)
		{
			//UE_LOG(LogTemp, Warning, TEXT("%f,  %f,  %f"), GameInstance->PathArray2[i].X, GameInstance->PathArray2[i].Y, GameInstance->PathArray2[i].Z);
			//상수는 총의 위치를 맞추기 위한 값
			PathMeshArray[i]->SetRelativeLocation(FVector(GameInstance->PathArray2[i].X + 230, GameInstance->PathArray2[i].Y - 13.0f, GameInstance->PathArray2[i].Z - 46.0f));
		}
		GameInstance->IsPathMade2 = false;
	}

	if (CurrentProjectile)
	{
		CurrentProjectile->SelectBulletSlot = 2;

		TArray<AActor*> network;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARBNetwork::StaticClass(), network);
		int32 size{};
		network.Empty(size);
		if (network.Num() > 0)
		{
			auto nt = Cast<ARBNetwork>(network[0]);
			if (nt)
			{
				if (nt->Get_Mode())
				{
					nt->SendBulletType((e_bulletType)GameInstance->SelectSlot[m_ID]);
				}
			}
		}
	}
}

void ARBCharacter::SelectSlot3()
{
	URBGameInstance* GameInstance = Cast<URBGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	GameInstance->SelectSlot[m_ID] = 3;

	if (GameInstance->IsPathMade3 == true)
	{
		for (int i = 0; i < 100; ++i)
		{
			//UE_LOG(LogTemp, Warning, TEXT("%f,  %f,  %f"), GameInstance->PathArray3[i].X, GameInstance->PathArray3[i].Y, GameInstance->PathArray3[i].Z);
			//상수는 총의 위치를 맞추기 위한 값
			PathMeshArray[i]->SetRelativeLocation(FVector(GameInstance->PathArray3[i].X + 230, GameInstance->PathArray3[i].Y - 13.0f, GameInstance->PathArray3[i].Z - 46.0f));
		}
		GameInstance->IsPathMade3 = false;
	}

	if (CurrentProjectile)
	{
		CurrentProjectile->SelectBulletSlot = 3;

		TArray<AActor*> network;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARBNetwork::StaticClass(), network);
		int32 size{};
		network.Empty(size);
		if (network.Num() > 0)
		{
			auto nt = Cast<ARBNetwork>(network[0]);
			if (nt)
			{
				if (nt->Get_Mode())
				{
					nt->SendBulletType((e_bulletType)GameInstance->SelectSlot[m_ID]);
				}
			}
		}
	}
}

void ARBCharacter::SelectSlot1(int id)
{
	URBGameInstance* GameInstance = Cast<URBGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	GameInstance->SelectSlot[id] = 1;

	//if (GameInstance->IsPathMade1 == true)
	//{
	//	for (int i = 0; i < 100; ++i)
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("%f,  %f,  %f"), GameInstance->PathArray1[i].X, GameInstance->PathArray1[i].Y, GameInstance->PathArray1[i].Z);
	//		//상수는 총의 위치를 맞추기 위한 값
	//		PathMeshArray[i]->SetRelativeLocation(FVector(GameInstance->PathArray1[i].X + 230, GameInstance->PathArray1[i].Y - 13.0f, GameInstance->PathArray1[i].Z - 46.0f));
	//	}
	//	GameInstance->IsPathMade1 = false;
	//}

	if (CurrentProjectile)
	{
		CurrentProjectile->SelectBulletSlot = 1;
	}
}

void ARBCharacter::SelectSlot2(int id)
{
	URBGameInstance* GameInstance = Cast<URBGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	GameInstance->SelectSlot[id] = 2;

	//if (GameInstance->IsPathMade1 == true)
	//{
	//	for (int i = 0; i < 100; ++i)
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("%f,  %f,  %f"), GameInstance->PathArray1[i].X, GameInstance->PathArray1[i].Y, GameInstance->PathArray1[i].Z);
	//		//상수는 총의 위치를 맞추기 위한 값
	//		PathMeshArray[i]->SetRelativeLocation(FVector(GameInstance->PathArray1[i].X + 230, GameInstance->PathArray1[i].Y - 13.0f, GameInstance->PathArray1[i].Z - 46.0f));
	//	}
	//	GameInstance->IsPathMade1 = false;
	//}

	if (CurrentProjectile)
	{
		CurrentProjectile->SelectBulletSlot = 2;
	}
}

void ARBCharacter::SelectSlot3(int id)
{
	URBGameInstance* GameInstance = Cast<URBGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	GameInstance->SelectSlot[id] = 3;

	//if (GameInstance->IsPathMade1 == true)
	//{
	//	for (int i = 0; i < 100; ++i)
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("%f,  %f,  %f"), GameInstance->PathArray1[i].X, GameInstance->PathArray1[i].Y, GameInstance->PathArray1[i].Z);
	//		//상수는 총의 위치를 맞추기 위한 값
	//		PathMeshArray[i]->SetRelativeLocation(FVector(GameInstance->PathArray1[i].X + 230, GameInstance->PathArray1[i].Y - 13.0f, GameInstance->PathArray1[i].Z - 46.0f));
	//	}
	//	GameInstance->IsPathMade1 = false;
	//}

	if (CurrentProjectile)
	{
		CurrentProjectile->SelectBulletSlot = 3;
	}
}

void ARBCharacter::StartFire()
{
	URBGameInstance* GameInstance = Cast<URBGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	if (IsReloading == false && bMenu == true) {
		if (CurrentWeapon)
		{
			CurrentWeapon->StartFire();
		}
	}
}

void ARBCharacter::StopFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
	}
}

void ARBCharacter::Reload()
{
	URBGameInstance* GameInstance = Cast<URBGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	IsReloading = true;
	AudioComponent->SetSound(ReloadCue);
	AudioComponent->Play();

	if(CurrentWeapon)
	{
		CurrentWeapon->Reload();
		PlayAnimMontage(ReloadMontage, 1.0f);
	}

	TArray<AActor*> network;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARBNetwork::StaticClass(), network);
	if (network.Num() > 0)
	{
		auto nt = Cast<ARBNetwork>(network[0]);
		if (nt)
		{
			if (nt->m_ID == m_ID)
				nt->SendCharacterReloadState(m_ID);
		}
	}
}



// Called to bind functionality to input
void ARBCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MF", this, &ARBCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MR", this, &ARBCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &ARBCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &ARBCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ARBCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ARBCharacter::EndCrouch);
	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &ARBCharacter::BeginZoom);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &ARBCharacter::EndZoom);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ARBCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ARBCharacter::StopFire);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ARBCharacter::Reload);

	PlayerInputComponent->BindAction("Slot1", IE_Pressed, this, &ARBCharacter::SelectSlot1);
	PlayerInputComponent->BindAction("Slot2", IE_Pressed, this, &ARBCharacter::SelectSlot2);
	PlayerInputComponent->BindAction("Slot3", IE_Pressed, this, &ARBCharacter::SelectSlot3);

	PlayerInputComponent->BindAction("RemoteBullet", IE_Pressed, this, &ARBCharacter::RemoteBulletPressed);
	PlayerInputComponent->BindAction("RemoteBullet", IE_Released, this, &ARBCharacter::RemoteBulletReleased);

	PlayerInputComponent->BindAction("LightOnOff", IE_Pressed, this, &ARBCharacter::LightOnOff);

}

void ARBCharacter::LightOnOff()
{
	if (LightTurn == false) {
		SpotLightComp->SetVisibility(false);
		LightTurn = true;
	}
	else if (LightTurn == true)
	{
		SpotLightComp->SetVisibility(true);
		LightTurn = false;
	}

	TArray<AActor*> network;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARBNetwork::StaticClass(), network);
	if (network.Num() > 0)
	{
		auto nt = Cast<ARBNetwork>(network[0]);
		if (nt)
		{
			if (nt->m_ID == m_ID)
				nt->SendCharacterLightState(m_ID);
		}
	}
}

void ARBCharacter::RemoteBulletPressed()
{
	URBGameInstance* GameInstance = Cast<URBGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if(GameInstance->bAccessReverse1 == true)
		GameInstance->bReverseBullet1 = true;

	if (GameInstance->bAccessReverse2 == true)
		GameInstance->bReverseBullet2 = true;

	if (GameInstance->bAccessReverse3 == true)
		GameInstance->bReverseBullet3 = true;
	
}

void ARBCharacter::RemoteBulletReleased()
{
	URBGameInstance* GameInstance = Cast<URBGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (GameInstance->bAccessReverse1 == true)
		GameInstance->bReverseBullet1 = false;

	if (GameInstance->bAccessReverse2 == true)
		GameInstance->bReverseBullet2 = false;

	if (GameInstance->bAccessReverse3 == true)
		GameInstance->bReverseBullet3 = false;

}




FVector ARBCharacter::GetPawnViewLocation() const
{
	if (CameraComp)
	{
		return CameraComp->GetComponentLocation();
	}

	return Super::GetPawnViewLocation();
}



FRotator ARBCharacter::GetCamRotator()
{
	return CameraComp->GetRelativeRotation();
}


void ARBCharacter::BeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag(FName("Door"))) {
		UE_LOG(LogTemp, Warning, TEXT("door"));
		if (isKey)
		{
			UE_LOG(LogTemp, Warning, TEXT("HasKey"));
			URBGameInstance* GameInstance = Cast<URBGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
			GameInstance->openDoor = true;
		}
	}
	else if (OtherComp->ComponentHasTag(FName("Key")))
	{
		UE_LOG(LogTemp, Warning, TEXT("key"));
		isKey = true;
	}
	else {
		AudioComponent->SetSound(HitCue);
		AudioComponent->Play();
		URBGameInstance* GameInstance = Cast<URBGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		GameInstance->PlayerMaxHP[m_ID] -= 10;
		MaxHP = GameInstance->PlayerMaxHP[m_ID];


		if (!IsDead)
		{
			if (GameInstance->PlayerMaxHP[m_ID] <= 0.0f)
			{
				IsDead = true;
				auto animinstance = Cast<URBAnimInstance>(GetMesh()->GetAnimInstance());
				animinstance->IsDead = true;

				TArray<AActor*> network;
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARBNetwork::StaticClass(), network);
				if (network.Num() > 0)
				{
					auto nt = Cast<ARBNetwork>(network[0]);
					if (nt)
					{
						nt->SendCharacterDeadState(m_ID);
					}
				}
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("MaxHP : %f,  InstanceHP : %f"), MaxHP, GameInstance->PlayerMaxHP[m_ID]);
	}
}