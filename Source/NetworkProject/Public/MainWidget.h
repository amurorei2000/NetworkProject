// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

/**
 *
 */
UCLASS()
class NETWORKPROJECT_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UTextBlock* text_Health;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UTextBlock* text_Ammo;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UTextBlock* text_playerList;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UButton* btn_Exit;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	class ANetworkProjectCharacter* player;

	UFUNCTION()
	void ExitSession();
};
