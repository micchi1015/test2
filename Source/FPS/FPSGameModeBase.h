// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "FPSGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API AFPSGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
		//�Q�[���v���C�J�n���ɃR�[������郁�\�b�h
		virtual void StartPlay() override;

		UUserWidget* OurWidget;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
		TSubclassOf<UUserWidget>UserWidgetClass;

		UUserWidget* TimeWidget;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Time")
		TSubclassOf<UUserWidget>UserClass;
};
