// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomizeController.h"

ACustomizeController::ACustomizeController()
{
	
	
}

void ACustomizeController::BeginPlay()
{
	Super::BeginPlay();

	/*if (HUDWidget != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidget);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
			CurrentWidget->SetOwningPlayer(0);
		}
	}*/
}