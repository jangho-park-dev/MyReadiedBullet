// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"

UEnemyAnimInstance::UEnemyAnimInstance()
{
	CurrentPawnSpeed = 0.f;
	enemy_state = CharacterAnimState::PEACE;
}

void UEnemyAnimInstance::setState(CharacterAnimState state)
{
	enemy_state = state;

	switch (state)
	{
	case CharacterAnimState::PEACE:
		//UE_LOG(LogTemp, Error, TEXT("state changed : PEACE"));
		break;
	case CharacterAnimState::CHASE:
		UE_LOG(LogTemp, Error, TEXT("state changed : CHASE"));
		break;
	case CharacterAnimState::ATTACK:
		UE_LOG(LogTemp, Error, TEXT("state changed : ATTACK"));
		break;
	case CharacterAnimState::DEAD:
		UE_LOG(LogTemp, Error, TEXT("state changed : DEAD"));
		break;
	case CharacterAnimState::HIT:
		UE_LOG(LogTemp, Error, TEXT("state changed : HIT"));
		break;
	}
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (!::IsValid(Pawn)) return;
	

	CurrentPawnSpeed = Pawn->GetVelocity().Size();

	

	//UE_LOG(LogTemp, Error, TEXT("speed : %f"), CurrentPawnSpeed);
	//auto Character = Cast<ACharacter>(Pawn);
}