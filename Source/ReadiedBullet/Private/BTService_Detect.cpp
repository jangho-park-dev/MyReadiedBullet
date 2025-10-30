// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect.h"
#include "BarghestAIController.h"
#include "ReadiedBullet/RBCharacter.h"
#include "EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
//#include "DrawDebugHelpers.h"
#include "EnemyAnimInstance.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
		return;

	auto dead_test = Cast<AEnemyCharacter>(ControllingPawn);
	if (dead_test->MaxHP <= 0.01f)
	{
		OwnerComp.StopTree();
	}


	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();
	float DetectRadius = 600.f;

	if (World == nullptr)
		return;

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel6,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	if (bResult)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(ABarghestAIController::TargetKey, nullptr);

		for (FOverlapResult& OverlapResult : OverlapResults)
		{
			ARBCharacter* RBCharacter = Cast<ARBCharacter>(OverlapResult.GetActor());
			if (RBCharacter && RBCharacter->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(ABarghestAIController::TargetKey, RBCharacter);
				//DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
				//DrawDebugPoint(World, RBCharacter->GetActorLocation(), 10.f, FColor::Blue, false, 0.2f);
				//DrawDebugLine(World, ControllingPawn->GetActorLocation(), RBCharacter->GetActorLocation(), FColor::Blue, false, 0.2f);
				
				auto e_char = Cast<AEnemyCharacter>(ControllingPawn);
				auto casted_animinstance = Cast<UEnemyAnimInstance>(e_char->GetMesh()->GetAnimInstance());


				if (casted_animinstance != nullptr)
				{
					e_char->GetCharacterMovement()->MaxWalkSpeed = 600.f;
					//UE_LOG(LogTemp, Error, TEXT("MaxSpeed : %d"), e_char->GetCharacterMovement()->MaxWalkSpeed);

					casted_animinstance->setState(CharacterAnimState::CHASE);

					
				}

				return;
			}
		}
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(ABarghestAIController::TargetKey, nullptr);

		auto e_char = Cast<AEnemyCharacter>(ControllingPawn);
		auto casted_animinstance = Cast<UEnemyAnimInstance>(e_char->GetMesh()->GetAnimInstance());
		
		if (casted_animinstance != nullptr)
		{
			//e_char->GetCharacterMovement()->MaxWalkSpeed = 300.f;
			casted_animinstance->setState(CharacterAnimState::PEACE);
		}
	}

	//DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);


}