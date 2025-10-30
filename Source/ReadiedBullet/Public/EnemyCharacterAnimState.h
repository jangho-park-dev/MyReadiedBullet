// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"
#include "EnemyCharacterAnimState.generated.h"

UENUM(BlueprintType)
enum class CharacterAnimState : uint8
{
	PEACE UMETA(DisplayName = "PEACE"),
	CHASE UMETA(DisplayName = "CHASE"),
	ATTACK UMETA(DisplayName = "ATTACK"),
	HIT UMETA(DisplayName = "HIT"),
	DEAD UMETA(DisplayName = "DEAD"),
};

/**
 * 
 */
UCLASS()
class READIEDBULLET_API UEnemyCharacterAnimState : public UBlackboardKeyType_Enum
{
	GENERATED_BODY()
	
public:
};
