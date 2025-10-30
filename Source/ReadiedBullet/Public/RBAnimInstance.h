// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ReadiedBullet/ReadiedBullet.h"
#include "ReadiedBullet/RBCharacter.h"
#include "Animation/AnimInstance.h"
#include "RBAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class READIEDBULLET_API URBAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	URBAnimInstance();

	void setDead();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HP")
	bool IsDead;
};
