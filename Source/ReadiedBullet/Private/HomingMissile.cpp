// Fill out your copyright notice in the Description page of Project Settings.


#include "HomingMissile.h"

// Sets default values
AHomingMissile::AHomingMissile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Head = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshHead"));
	//Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshBody"));
	ProjectileMove = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	Particle = CreateDefaultSubobject<UParticleSystem>(TEXT("ParticleSys"));
	BoxColl = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));

	RootComponent = Head;
	homingTimer = 0.0f;
	static ConstructorHelpers::FObjectFinder<USoundBase>MISSILESOUND(TEXT("SoundWave'/Game/Sound/Missle_Launch.Missle_Launch'"));
	if (MISSILESOUND.Succeeded())
	{
		MissileCue = MISSILESOUND.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem>PARTICLE(TEXT("ParticleSystem'/Game/Effects/AdvancedMagicFX13/Particles/P_ky_explosion3.P_ky_explosion3'"));
	if (PARTICLE.Succeeded())
	{
		Particle = PARTICLE.Object;
	}

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio_Fire"));
	AudioComponent->bAutoActivate = false;
	AudioComponent->SetupAttachment(Body);

}

// Called when the game starts or when spawned
void AHomingMissile::BeginPlay()
{
	Super::BeginPlay();

	BoxColl->OnComponentBeginOverlap.AddDynamic(this, &AHomingMissile::BeginOverlap);
	ProjectileMove->bIsHomingProjectile = false;
}

// Called every frame
void AHomingMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	homingTimer += 1.0f;

	if (homingTimer > 200.0f && homingTimer < 202.0f)
	{
		ARBCharacter* playerCharacter = Cast<ARBCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		USceneComponent* target = playerCharacter->GetRootComponent();
		ProjectileMove->HomingTargetComponent = target;
		ProjectileMove->bIsHomingProjectile = true;
		homingTimer = 300.0f;
	}
}

void AHomingMissile::BeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, OverlappedComponent->GetComponentTransform());
	this->Destroy();
}