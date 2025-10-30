// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "CoreMinimal.h"
#include "RBGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Pawn.h"
#include "CusPawn.generated.h"

UCLASS()
class READIEDBULLET_API ACusPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACusPawn();
	FRotator GetSpringArmRotator();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PressedKeyboardW();
	void PressedKeyboardS();
	void PressedKeyboardA();
	void PressedKeyboardD();

	void MouseWheelUp();
	void MouseWheelDown();


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComp;




	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Transfrom")
	TArray<FTransform> TransformSlotBox1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Transfrom")
	TArray<FTransform> TransformSlotCylinder1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Transfrom")
	TArray<FTransform> TransformSlotSphere1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Transfrom")
	TArray<FTransform> TransformSlotBox2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Transfrom")
	TArray<FTransform> TransformSlotCylinder2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Transfrom")
	TArray<FTransform> TransformSlotSphere2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Transfrom")
	TArray<FTransform> TransformSlotBox3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Transfrom")
	TArray<FTransform> TransformSlotCylinder3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Transfrom")
	TArray<FTransform> TransformSlotSphere3;

	FRotator SpringArmRotator;
	FVector CamVector;

	UPROPERTY(EditAnywhere)
	FName Level1ToLoad;

	UPROPERTY(EditAnywhere)
	FName Level2ToLoad;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void SaveCustomCam();

	UFUNCTION(BlueprintCallable)
	void SaveGameInstance();
	
};
