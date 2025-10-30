// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ReadiedBullet/ReadiedBullet.h"
#include "AIController.h"
#include "BarghestAIController.generated.h"


/**
 * 
 */
UCLASS()
class READIEDBULLET_API ABarghestAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ABarghestAIController();
	virtual void OnPossess(APawn* InPawn) override;

	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;

private:
	UPROPERTY()
	class UBehaviorTree* BTAsset;

	UPROPERTY()
	class UBlackboardData* BBAsset;
};
