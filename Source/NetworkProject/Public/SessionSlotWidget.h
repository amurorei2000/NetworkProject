// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SessionSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKPROJECT_API USessionSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	class UButton* btn_roomName;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* text_roomName;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* text_playerInfo;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* text_ping;

	UPROPERTY()
	int32 index;

protected:
	virtual void NativeConstruct() override;

private:
	class UServerGameInstance* gameInstance;

	UFUNCTION()
	void JoinRoom();
};
