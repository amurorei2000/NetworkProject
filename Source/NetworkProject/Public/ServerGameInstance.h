// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "ServerGameInstance.generated.h"

/**
 * 
 */

 USTRUCT()
 struct FSessionInfo
 {
	GENERATED_BODY()

	FString roomName;
	int32 currentPlayers;
	int32 maxPlayers;
	int32 ping;
	int32 idx;
 };


 DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSearchResult, FSessionInfo, sessionInfo);

 DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSearchFinished);

UCLASS()
class NETWORKPROJECT_API UServerGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UServerGameInstance();
	virtual void Init() override;

	// 전역 변수
	IOnlineSessionPtr sessionInterface;
	FName sessionID;
	TSharedPtr<FOnlineSessionSearch> sessionSearch;
	FOnSearchResult searchResultDele;
	FOnSearchFinished searchFinishedDele;

	// 함수
	void CreateMySession(FString roomName, int32 playerCount);
	void FindMySession();
	void JoinMySession(int32 sessionIdx);

	
	UFUNCTION()
	void OnCreateSessionComplete(FName sessionName, bool bIsSuccess);

	UFUNCTION()
	void OnFindSessionComplete(bool bWasSuccessful);
	
	void OnJoinSessionComplete(FName sessionName, EOnJoinSessionCompleteResult::Type joinResult);

};
