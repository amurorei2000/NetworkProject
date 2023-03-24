// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginWidget.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKPROJECT_API ULoginWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	class UEditableText* editText_id;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	class UButton* btn_Start;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	class UEditableText* editText_roomName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	class USlider* sl_playerCount;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* text_playerCount;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	class UButton* btn_CreateSession;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	class UWidgetSwitcher* widgetSwitcher;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	class UButton* btn_GoCreate;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	class UButton* btn_GoFind;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	class UButton* btn_Refresh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	class UButton* btn_CreateBack;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	class UButton* btn_FindBack;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	class UScrollBox* sbox_RoomList;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class USessionSlotWidget> sessionSlot;

protected:
	virtual void NativeConstruct() override;

private:
	class UServerGameInstance* gameInstance;

	UFUNCTION()
	void ClickStart();
	
	UFUNCTION()
	void CreateServer();

	UFUNCTION()
	void OnMoveSlider(float Value);

	UFUNCTION()
	void GoCreate();

	UFUNCTION()
	void GoFind();

	UFUNCTION()
	void GoBack();

	UFUNCTION()
	void AddNewSlot(FSessionInfo sessionInfo);

	UFUNCTION()
	void RefreshList();

	UFUNCTION()
	void RefreshEnabled();

};
