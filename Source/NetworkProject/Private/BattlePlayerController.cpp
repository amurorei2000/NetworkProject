// Fill out your copyright notice in the Description page of Project Settings.


#include "BattlePlayerController.h"
#include "MainWidget.h"


void ABattlePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (mainWidget != nullptr && IsLocalController())
	{
		UMainWidget* mainUI = CreateWidget<UMainWidget>(this, mainWidget);

		if (mainUI)
		{
			mainUI->AddToViewport();
		}
	}

}
