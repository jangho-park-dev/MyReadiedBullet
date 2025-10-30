// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ReadiedBullet.h"
#include "GameFramework/PlayerController.h"
#include "RBPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class READIEDBULLET_API ARBPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void PostInitializeComponents() override;  //폰과 컨트롤러가 생성되는 시점
	virtual void OnPossess(APawn* aPawn) override;
protected:
	virtual void BeginPlay() override;

};

