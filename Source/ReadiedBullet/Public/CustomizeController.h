// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "CustomizeController.generated.h"

/**
 * 
 */
UCLASS()
class READIEDBULLET_API ACustomizeController : public APlayerController
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACustomizeController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<class UUserWidget> HUDWidget;

	UPROPERTY()
	class UUserWidget* CurrentWidget;

};
