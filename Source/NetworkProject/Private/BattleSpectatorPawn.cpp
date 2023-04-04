// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleSpectatorPawn.h"
#include "BattlePlayerController.h"
#include "../NetworkProjectCharacter.h"


void ABattleSpectatorPawn::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle respawnTimer;
	GetWorldTimerManager().SetTimer(respawnTimer, this, &ABattleSpectatorPawn::Change2Player, 3.0f, false);

}

void ABattleSpectatorPawn::Change2Player()
{
	if (HasAuthority())
	{
		ABattlePlayerController* pc = Cast<ABattlePlayerController>(GetController());
		if (pc != nullptr && originalPlayer != nullptr)
		{
			pc->Possess(originalPlayer);
			pc->Respawn(originalPlayer);
		}
	}
}
