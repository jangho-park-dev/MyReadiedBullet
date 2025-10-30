// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterWidget.h"

void UMonsterWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HPbar")));
	HPProgressBar->SetPercent(1.0f);
}