// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "ReadiedBullet/ReadiedBullet.h"
//#include "SHealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Classes/Components/InputComponent.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBarWidget"));
	/*SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);*/
	P_Die = CreateDefaultSubobject<UParticleSystem>(TEXT("DieParticle"));
	HPBarWidget->SetupAttachment(GetMesh());
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);

	//HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComp"));
	
	/*CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);*/

	/*ZoomedFOV = 65.0f;
	ZoomInterpSpeed = 20;*/

	static ConstructorHelpers::FObjectFinder<UAnimSequence> DeathAnim(TEXT("AnimSequence'/Game/Animation/Animation_Honet/Death_1.Death_1'"));
	S_DeathAnim = DeathAnim.Object;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> Particle(TEXT("ParticleSystem'/Game/Effects/AdvancedMagicFX13/Particles/P_ky_gaither.P_ky_gaither'"));
	P_Die = Particle.Object;

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/UI/WBP_HPBar"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(200.0f, 80.0));
	}

	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	MaxHP = 1.0f;
	WeaponAttachSocketName = "WeaponSocket";
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ASCharacter::BeginOverlap);
	//DefaultFOV = CameraComp->FieldOfView;
	//HealthComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
	if (Role == ROLE_Authority)
	{
		// Spawn a default weapon
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		UE_LOG(LogTemp, Log, TEXT("1 : spawn Prams"));

		CurrentWeapon = GetWorld()->SpawnActor<ASWeapon>(StarterWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		UE_LOG(LogTemp, Log, TEXT("2 : load Current Weapon"));
		if (CurrentWeapon)
		{
			CurrentWeapon->SetOwner(this);
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);
			UE_LOG(LogTemp, Log, TEXT("3 : if there is Current Weapon, attach it"));
		}
	}
}

void ASCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);
}


void ASCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}


void ASCharacter::BeginCrouch()
{
	Crouch();
}


void ASCharacter::EndCrouch()
{
	UnCrouch();
}


void ASCharacter::BeginZoom()
{
	bWantsToZoom = true;
}


void ASCharacter::EndZoom()
{
	bWantsToZoom = false;
}


void ASCharacter::StartFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StartFire();
	}
}


void ASCharacter::StopFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
	}
}

//void ASCharacter::OnHealthChanged(USHealthComponent* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType,
//	class AController* InstigatedBy, AActor* DamageCauser)
//{
//	if (Health <= 0.0f && !bDied)
//	{
//		// Die!
//		bDied = true;
//
//		GetMovementComponent()->StopMovementImmediately();
//		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//
//		DetachFromControllerPendingDestroy();
//
//		SetLifeSpan(10.0f);
//	}
//}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//float TargetFOV = bWantsToZoom ? ZoomedFOV : DefaultFOV;
	//float NewFOV = FMath::FInterpTo(CameraComp->FieldOfView, TargetFOV, DeltaTime, ZoomInterpSpeed);

	//CameraComp->SetFieldOfView(NewFOV);
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &ASCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &ASCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ASCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ASCharacter::EndCrouch);

	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &ASCharacter::BeginZoom);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &ASCharacter::EndZoom);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASCharacter::StopFire);

	// CHALLENGE CODE
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
}

FVector ASCharacter::GetPawnViewLocation() const
{
	/*if (CameraComp)
	{
		return CameraComp->GetComponentLocation();
	}*/

	return Super::GetPawnViewLocation();
}


void ASCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASCharacter, CurrentWeapon);
	DOREPLIFETIME(ASCharacter, bDied);
}

void ASCharacter::BeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	//HPBarWidget->SetVisibility(true);
	UMonsterWidget* MW = Cast<UMonsterWidget>(HPBarWidget->GetUserWidgetObject());
	MaxHP -= 0.2f;
	MW->HPProgressBar->SetPercent(MaxHP);

	if (MaxHP < 0.01)
	{
		//여기다가 죽는거 플레이
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), P_Die, GetMesh()->GetComponentTransform(), true, EPSCPoolMethod::None, true);
		CurrentWeapon->Destroy();
		this->Destroy();
	}
}