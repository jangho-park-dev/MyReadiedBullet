// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RBGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class READIEDBULLET_API URBGameInstance : public UGameInstance
{
	GENERATED_BODY()
	

public:
	//URBGameInstance();
	void SetInstanceBoxSlot1(TArray<FTransform> BoxSlot);
	void SetInstanceCylinderSlot1(TArray<FTransform> CylinderSlot);
	void SetInstanceSphereSlot1(TArray<FTransform> SphereSlot);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Instance")
	TArray<FTransform>  InstanceBoxSlot1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Instance")
	TArray<FTransform>  InstanceCylinderSlot1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Instance")
	TArray<FTransform>  InstanceSphereSlot1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Instance")
	TArray<FTransform>  InstanceBoxSlot2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Instance")
	TArray<FTransform>  InstanceCylinderSlot2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Instance")
	TArray<FTransform>  InstanceSphereSlot2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Instance")
	TArray<FTransform>  InstanceBoxSlot3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Instance")
	TArray<FTransform>  InstanceCylinderSlot3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Instance")
	TArray<FTransform>  InstanceSphereSlot3;

	TArray<FTransform> GetInstanceBoxSlot1();
	TArray<FTransform> GetInstanceCylinderSlot1();
	TArray<FTransform> GetInstanceSphereSlot1();

	////////////////////////
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Instance")
	TArray<float> SaveSlot1_InstanceX = { 0,0,0,0,0,0 };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Instance")
	TArray<float> SaveSlot1_InstanceY = { 0,0,0,0,0,0 };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Instance")
	TArray<float> SaveSlot1_InstanceZ = { 0,0,0,0,0,0 };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Instance")
	TArray<float> SaveSlot2_InstanceX = { 0,0,0,0,0,0 };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Instance")
	TArray<float> SaveSlot2_InstanceY = { 0,0,0,0,0,0 };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Instance")
	TArray<float> SaveSlot2_InstanceZ = { 0,0,0,0,0,0 };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Instance")
	TArray<float> SaveSlot3_InstanceX = { 0,0,0,0,0,0 };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Instance")
	TArray<float> SaveSlot3_InstanceY = { 0,0,0,0,0,0 };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Instance")
	TArray<float> SaveSlot3_InstanceZ = { 0,0,0,0,0,0 };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SelectSlot")
	TArray<int32> SelectSlot = { 1, 1, 1, 1, 1, 1 };
	//////////////////////

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Instance")
	TArray<float> PlayerMaxHP = { 100.0f, 100.0f, 100.0f, 100.0f, 100.0f, 100.0f };


	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Instance")
	int32 BulletImpactSolt1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Instance")
	int32 BulletImpactSolt2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Instance")
	int32 BulletImpactSolt3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Armo")
	int32 CurrentArmo = 30;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Armo")
	int32 Margazine = 90;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Armo")
	bool IsReloading;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Path")
	TArray<FVector> PathArray1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Path")
	TArray<FVector> PathArray2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Path")
	TArray<FVector> PathArray3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Path")
	bool IsPathMade1 = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Path")
	bool IsPathMade2 = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Path")
	bool IsPathMade3 = false;

	bool bReverseBullet1 = false;
	bool bReverseBullet2 = false;
	bool bReverseBullet3 = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Reverse")
	bool bAccessReverse1 = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Reverse")
	bool bAccessReverse2 = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Reverse")
	bool bAccessReverse3 = false;



	bool bBlackHoleBullet1 = false;
	bool bBlackHoleBullet2 = false;
	bool bBlackHoleBullet3 = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Blackhole")
	bool bAccessBlackhole1 = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Blackhole")
	bool bAccessBlackhole2 = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Blackhole")
	bool bAccessBlackhole3 = false;
	

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cinema")
	bool isCompleteCinema = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Door")
	bool openDoor = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Door")
	bool openDoorComplete = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Door")
	bool Boss1Die = false;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stage")
	int CurrentStage = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stage")
	bool clearStage1 = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stage")
	bool clearStage2 = false;

};
