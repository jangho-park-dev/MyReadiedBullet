// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

AEnemyAIController::AEnemyAIController()
{
	RepeatInterval = 3.f;
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &AEnemyAIController::OnRepeatTimer, RepeatInterval, true);
}

//void AEnemyAIController::UnPossess()
//{
//	Super::UnPossess();
//	GetWorld()->GetTimerManager().ClearTimer(RepeatTimerHandle);
//}

void AEnemyAIController::OnRepeatTimer()
{
	auto CurrentPawn = GetPawn();

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	if (NavSystem == nullptr) return;

	FNavLocation NextLocation;
	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.f, NextLocation))
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, NextLocation.Location);
	}
}