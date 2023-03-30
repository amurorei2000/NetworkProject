// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInfoWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "../NetworkProjectCharacter.h"

void UPlayerInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	myPlayer = Cast<ANetworkProjectCharacter>(GetOwningPlayerPawn());

	if (myPlayer != nullptr)
	{
		ServerSetHealthBar(myPlayer->GetHealth());
	}
}

void UPlayerInfoWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(myPlayer == nullptr)
	return;
	
}

void UPlayerInfoWidget::SetPlayer(class ANetworkProjectCharacter* player)
{
	myPlayer = player;
}

void UPlayerInfoWidget::ServerSetHealthBar_Implementation(const int32& value)
{
	MulticastSetHealthBar(value);
}

void UPlayerInfoWidget::MulticastSetHealthBar_Implementation(const int32& value)
{
	float calcHP = (float)(myPlayer->GetHealth()) / (float)myPlayer->maxHP;
	pb_HP->SetPercent(calcHP);
}
