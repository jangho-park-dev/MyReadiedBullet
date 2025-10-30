// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TurnToTarget.h"
#include "BarghestAIController.h"
#include "EnemyCharacter.h"
#include "ReadiedBullet/RBCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto EnemyCharacter = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (EnemyCharacter == nullptr)
		return EBTNodeResult::Failed;

	auto Target = Cast<ARBCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ABarghestAIController::TargetKey));
	if (Target == nullptr)
		return EBTNodeResult::Failed;

	FVector LookVector = Target->GetActorLocation() - EnemyCharacter->GetActorLocation();
	LookVector.Z = 0.f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	EnemyCharacter->SetActorRotation(FMath::RInterpTo(EnemyCharacter->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.f));

	return EBTNodeResult::Succeeded;
}
