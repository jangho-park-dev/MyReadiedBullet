// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ReadiedBullet/ReadiedBullet.h"
#include "Animation/AnimInstance.h"
#include "EnemyCharacterAnimState.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class READIEDBULLET_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UEnemyAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void setState(CharacterAnimState state);
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn, Meta = (AllowPrivateAccess = true))
	CharacterAnimState enemy_state;
};
