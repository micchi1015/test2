// Copyright Epic Games, Inc. All Rights Reserved.


#include "FPSGameModeBase.h"

//�Q�[���v���C�J�n���ɃR�[������郁�\�b�h
void AFPSGameModeBase::StartPlay()
{
	Super::StartPlay();

	////�f�o�b�O�����̕\��
	//GEngine->AddOnScreenDebugMessage((uint64)-1,	//Key
	//	2.0f,										//TimeToDisplay
	//	FLinearColor::Red.ToFColor(true),			//DisplayColor
	//	TEXT("Hello World!"));						//DebugString

	//USphereComponent�𐶐�
	OurWidget = CreateWidget<UUserWidget>(GetWorld(), UserWidgetClass);

	OurWidget->AddToViewport();

	//USphereComponent�𐶐�
	TimeWidget = CreateWidget<UUserWidget>(GetWorld(), UserClass);

	TimeWidget->AddToViewport();
}