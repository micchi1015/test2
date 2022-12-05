// Copyright Epic Games, Inc. All Rights Reserved.


#include "FPSGameModeBase.h"

//ゲームプレイ開始時にコールされるメソッド
void AFPSGameModeBase::StartPlay()
{
	Super::StartPlay();

	////デバッグ文字の表示
	//GEngine->AddOnScreenDebugMessage((uint64)-1,	//Key
	//	2.0f,										//TimeToDisplay
	//	FLinearColor::Red.ToFColor(true),			//DisplayColor
	//	TEXT("Hello World!"));						//DebugString

	//USphereComponentを生成
	OurWidget = CreateWidget<UUserWidget>(GetWorld(), UserWidgetClass);

	OurWidget->AddToViewport();

	//USphereComponentを生成
	TimeWidget = CreateWidget<UUserWidget>(GetWorld(), UserClass);

	TimeWidget->AddToViewport();
}