// Fill out your copyright notice in the Description page of Project Settings.


#include "BattlePlayerController.h"
#include "MainWidget.h"
#include "BattleGameMode.h"
#include "../NetworkProjectCharacter.h"


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

// 캐릭터 부활 함수
void ABattlePlayerController::Respawn(ANetworkProjectCharacter* player)
{
	if (HasAuthority() && player != nullptr)
	{
		ABattleGameMode* gm = Cast< ABattleGameMode>(GetWorld()->GetAuthGameMode());
		if (gm != nullptr)
		{
			player->Destroy();
			gm->RestartPlayer(this);
		}
	}
}
