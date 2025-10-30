// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ReadiedBullet.h"
#include "GameFramework/GameModeBase.h"
#include "ReadiedBulletGameMode.generated.h"

UCLASS(minimalapi)
class AReadiedBulletGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AReadiedBulletGameMode();
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override; //플레이어가 로그인을 완료하면 호출되는 이벤트 함수

	


};



