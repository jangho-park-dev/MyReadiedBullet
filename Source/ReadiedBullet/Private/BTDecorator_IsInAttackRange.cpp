// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsInAttackRange.h"
#include "BarghestAIController.h"
#include "EnemyCharacter.h"
#include "ReadiedBullet/RBCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr)
		return false;

	auto Target = Cast<ARBCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ABarghestAIController::TargetKey));
	if (Target == nullptr)
		return false;

	auto e_char = Cast<AEnemyCharacter>(ControllingPawn);

	if (e_char->eName == enemyCharacterName::Dragon)
	{
		bResult = (Target->GetDistanceTo(ControllingPawn) <= ControllingPawn->GetFinalAttackRange_Dragon());
	}
	else
	{
		bResult = (Target->GetDistanceTo(ControllingPawn) <= ControllingPawn->GetFinalAttackRange());
	}
	


	UE_LOG(LogTemp, Error, TEXT("in attack range : %d"), bResult);

	return bResult;
}