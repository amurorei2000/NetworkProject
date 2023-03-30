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
		SetHealthBar(myPlayer->GetHealth());
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

void UPlayerInfoWidget::SetHealthBar(const int32& value)
{
	if(myPlayer !=nullptr)
	{
		float calcHP = (float)value / (float)myPlayer->maxHP;
		pb_HP->SetPercent(calcHP);
	}
}

