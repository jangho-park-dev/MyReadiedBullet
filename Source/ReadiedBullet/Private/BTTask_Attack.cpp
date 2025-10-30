// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "BarghestAIController.h"
#include "EnemyCharacter.h"
#include "EnemyAnimInstance.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;
	IsAttacking = false;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto EnemyCharacter = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (EnemyCharacter == nullptr)
		return EBTNodeResult::Failed;


	/* attack animation & attack end check 추가했음 */
	auto casted_animinstance = Cast<UEnemyAnimInstance>(EnemyCharacter->GetMesh()->GetAnimInstance());
	casted_animinstance->setState(CharacterAnimState::ATTACK);

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}