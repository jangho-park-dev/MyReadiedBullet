// Fill out your copyright notice in the Description page of Project Settings.


#include "CusPawn.h"

// Sets default values
ACusPawn::ACusPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));


	RootComponent = StaticMeshComp;
	SpringArmComp->SetupAttachment(StaticMeshComp);
	CameraComp->SetupAttachment(SpringArmComp);

	SpringArmComp->bEnableCameraLag = false;
	SpringArmComp->bEnableCameraRotationLag = true;

	SpringArmRotator.Pitch = 270.0f;
	SpringArmRotator.Yaw = 0.0f;
	SpringArmRotator.Roll = 0.0f;

	SpringArmComp->SetWorldRotation(SpringArmRotator);
	CameraComp->SetWorldLocation(CamVector);


}

// Called when the game starts or when spawned
void ACusPawn::BeginPlay()
{
	Super::BeginPlay();
	

	CamVector.X = -900.0f;
	CamVector.Y = 0.0f;
	CamVector.Z = 0.0f;
}

void ACusPawn::PressedKeyboardW()
{
	SpringArmRotator.Pitch -= 90.0f;
	if (SpringArmRotator.Pitch < 0.0f)
	{
		SpringArmRotator.Pitch = 270.0f;
	}
	SpringArmComp->SetWorldRotation(SpringArmRotator);
	UE_LOG(LogTemp, Log, TEXT("%f %f %f"), SpringArmRotator.Roll, SpringArmRotator.Pitch, SpringArmRotator.Yaw);
}

void ACusPawn::PressedKeyboardS()
{
	SpringArmRotator.Pitch += 90.0f;
	if (SpringArmRotator.Pitch > 360.0f)
	{
		SpringArmRotator.Pitch = 90.0f;
	}
	SpringArmComp->SetWorldRotation(SpringArmRotator);
	UE_LOG(LogTemp, Log, TEXT("%f %f %f"), SpringArmRotator.Roll, SpringArmRotator.Pitch, SpringArmRotator.Yaw);
}

void ACusPawn::PressedKeyboardA()
{
	if (SpringArmRotator.Pitch == 180.0f || SpringArmRotator.Pitch == 360.0f || SpringArmRotator.Pitch == 0.0f) 
	{
		SpringArmRotator.Yaw += 90.0f;
		if (SpringArmRotator.Yaw > 360.0f)
		{
			SpringArmRotator.Yaw = 90.0f;
		}
	}

	if (SpringArmRotator.Pitch == 270.0f || SpringArmRotator.Pitch == 90.0f)
	{
		SpringArmRotator.Yaw += 90.0f;
		if (SpringArmRotator.Yaw > 360.0f)
		{
			SpringArmRotator.Yaw = 90.0f;
		}
	}
	

	SpringArmComp->SetWorldRotation(SpringArmRotator);
	UE_LOG(LogTemp, Log, TEXT("%f %f %f"), SpringArmRotator.Roll, SpringArmRotator.Pitch, SpringArmRotator.Yaw);
}

void ACusPawn::PressedKeyboardD()
{
	if (SpringArmRotator.Pitch == 180.0f || SpringArmRotator.Pitch == 360.0f || SpringArmRotator.Pitch == 0.0f)
	{
		SpringArmRotator.Yaw -= 90.0f;
		if (SpringArmRotator.Yaw < 0.0f)
		{
			SpringArmRotator.Yaw = 270.0f;
		}
	}

	if (SpringArmRotator.Pitch == 270.0f || SpringArmRotator.Pitch == 90.0f)
	{
		SpringArmRotator.Yaw -= 90.0f;
		if (SpringArmRotator.Yaw < 0.0f)
		{
			SpringArmRotator.Yaw = 270.0f;
		}
	}
	SpringArmComp->SetWorldRotation(SpringArmRotator);
	UE_LOG(LogTemp, Log, TEXT("%f %f %f"), SpringArmRotator.Roll, SpringArmRotator.Pitch, SpringArmRotator.Yaw);
}

void ACusPawn::MouseWheelUp()
{
	//CamVector.X = CamVector.X + 10;
	//CameraComp->AddRelativeLocation(CamVector);
}

void ACusPawn::MouseWheelDown()
{
	//CamVector.X = CamVector.X - 10;
	//CameraComp->AddRelativeLocation(CamVector);
}

FRotator ACusPawn::GetSpringArmRotator()
{
	return SpringArmRotator;
}

// Called every frame
void ACusPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//URBGameInstance* GameInstance = Cast<URBGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	//if (GameInstance->clearStage1)
	//{
	//	GameInstance->clearStage1 = false;
	//	FLatentActionInfo LatentInfo;
	//	UGameplayStatics::LoadStreamLevel(this, Level1ToLoad, true, true, LatentInfo);
	//	UGameplayStatics::OpenLevel(this, Level1ToLoad, true, "");
	//}

	//if (GameInstance->clearStage2)
	//{
	//	GameInstance->clearStage2 = false;
	//	FLatentActionInfo LatentInfo;
	//	UGameplayStatics::LoadStreamLevel(this, Level2ToLoad, true, true, LatentInfo);
	//}

}

// Called to bind functionality to input
void ACusPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("KeyW", IE_Pressed, this, &ACusPawn::PressedKeyboardW);
	PlayerInputComponent->BindAction("KeyS", IE_Pressed, this, &ACusPawn::PressedKeyboardS);
	PlayerInputComponent->BindAction("KeyA", IE_Pressed, this, &ACusPawn::PressedKeyboardA);
	PlayerInputComponent->BindAction("KeyD", IE_Pressed, this, &ACusPawn::PressedKeyboardD);

	PlayerInputComponent->BindAction("WheelUp", IE_Pressed, this, &ACusPawn::MouseWheelUp);
	PlayerInputComponent->BindAction("WheelDown", IE_Pressed, this, &ACusPawn::MouseWheelDown);

}

void ACusPawn::SaveCustomCam()
{
	

	
}

void ACusPawn::SaveGameInstance()
{
	URBGameInstance* GameInstance = Cast<URBGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	GameInstance->InstanceBoxSlot1= TransformSlotBox1;
	GameInstance->InstanceCylinderSlot1 = TransformSlotCylinder1;
	GameInstance->InstanceSphereSlot1 = TransformSlotSphere1;

	GameInstance->InstanceBoxSlot2 = TransformSlotBox2;
	GameInstance->InstanceCylinderSlot2 = TransformSlotCylinder2;
	GameInstance->InstanceSphereSlot2 = TransformSlotSphere2;

	GameInstance->InstanceBoxSlot3 = TransformSlotBox3;
	GameInstance->InstanceCylinderSlot3 = TransformSlotCylinder3;
	GameInstance->InstanceSphereSlot3 = TransformSlotSphere3;
}

