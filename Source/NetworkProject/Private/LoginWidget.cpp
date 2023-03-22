// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/Slider.h"
#include "Components/WidgetSwitcher.h"
#include "ServerGameInstance.h"


void ULoginWidget::NativeConstruct()
{
	// Ȥ�� �� �Է� �ʵ��� ���� �� ĭ���� �ʱ�ȭ�Ѵ�.
	editText_id->SetText(FText::FromString(""));
	editText_roomName->SetText(FText::FromString(""));
	sl_playerCount->SetValue(2.0f);
	text_playerCount->SetText(FText::FromString("2"));

	// ��ư Ŭ�� �̺�Ʈ�� �Լ� ���ε��ϱ�
	btn_Start->OnClicked.AddDynamic(this, &ULoginWidget::ClickStart);
	btn_CreateSession->OnClicked.AddDynamic(this, &ULoginWidget::CreateServer);
	sl_playerCount->OnValueChanged.AddDynamic(this, &ULoginWidget::OnMoveSlider);

	gameInstance = Cast<UServerGameInstance>(GetGameInstance());
}

void ULoginWidget::ClickStart()
{
	// ����, ID�� �� ĭ�� �ƴ϶�� 0�� -> 1�� ĵ������ �����Ѵ�.
	if (!editText_id->GetText().IsEmpty())
	{
		widgetSwitcher->SetActiveWidgetIndex(1);
		gameInstance->sessionID = FName(*editText_id->GetText().ToString());
	}
}

void ULoginWidget::CreateServer()
{
	int32 playerCnt = FMath::RoundHalfFromZero(sl_playerCount->GetValue());

	gameInstance->CreateMySession(editText_roomName->GetText().ToString(), playerCnt);
}

// �����̴� ���� ����� �� ȣ��Ǵ� �Լ�
void ULoginWidget::OnMoveSlider(float Value)
{
	FString num2str = FString::Printf(TEXT("%d"), Value);
	text_playerCount->SetText(FText::FromString(num2str));
}
