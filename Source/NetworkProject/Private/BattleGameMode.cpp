// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"


AActor* ABattleGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	// 월드 안에 있는 APlayerStart 액터들을 가져온다.
	for (TActorIterator<APlayerStart> iter(GetWorld()); iter; ++iter)
	{
		APlayerStart* ps = *iter;
		if (ps->PlayerStartTag != FName("Spawned"))
		{
			// 반환될 APlayerStart 액터의 태그를 "Spawned" 추가한다.
			ps->PlayerStartTag = FName("Spawned");
			return ps;
		}
	}
	
	return nullptr;
	
}


