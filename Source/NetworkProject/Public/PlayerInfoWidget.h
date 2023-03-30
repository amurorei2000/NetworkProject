// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKPROJECT_API UPlayerInfoWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* text_Name;

	UPROPERTY(meta=(BindWidget))
	class UProgressBar* pb_HP;

	void SetPlayer(class ANetworkProjectCharacter* player);

	void SetHealthBar(const int32& value);



private:
	class ANetworkProjectCharacter* myPlayer;

};
