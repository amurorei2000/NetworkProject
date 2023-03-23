// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerGameInstance.h"
#include "OnlineSubSystem.h"
#include "OnlineSessionSettings.h"

// 생성자
UServerGameInstance::UServerGameInstance()
{
	sessionID = "Test Session";
}

// 최초 실행되는 함수
void UServerGameInstance::Init()
{
	Super::Init();

	// 온라인 세션 기능이 구현되어 있는 IOnlineSubsystem 클래스를 가져온다.
	IOnlineSubsystem* subsys = IOnlineSubsystem::Get();
	if (subsys)
	{
		sessionInterface = subsys->GetSessionInterface();
		
		if (sessionInterface != nullptr)
		{
			sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UServerGameInstance::OnCreateSessionComplete);
		}
	}
}

// 세션 생성 함수
void UServerGameInstance::CreateMySession(FString roomName, int32 playerCount)
{
	if (sessionInterface != nullptr)
	{
		// 세션 생성 정보를 만든다.
		FOnlineSessionSettings sessionSettings;
		sessionSettings.bAllowInvites = false;
		sessionSettings.bAllowJoinInProgress = true;
		sessionSettings.bAllowJoinViaPresence = true;
		sessionSettings.bShouldAdvertise = true;
		sessionSettings.bIsDedicated = false;
		// 온라인 서브시스템의 이름이 "NULL"이면 LAN 매칭을 하고, "Steam"이면 스팀 서버로 매칭한다.
		sessionSettings.bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
		sessionSettings.NumPublicConnections = playerCount;
		sessionSettings.Set(FName("KEY_RoomName"), roomName);

		// 세션을 생성한다.
		sessionInterface->CreateSession(0, sessionID, sessionSettings);

		UE_LOG(LogTemp, Warning, TEXT("Create Session try!"));
	}
}

// 세션이 서버에 만들어졌을 때 호출된 함수
void UServerGameInstance::OnCreateSessionComplete(FName sessionName, bool bIsSuccess)
{
	FString result = bIsSuccess ? TEXT("Create Session Success!") : TEXT("Create Session Failed...");
	UE_LOG(LogTemp, Warning, TEXT("%s : %s"), *result, *sessionName.ToString());

	// 세션 생성에 성공했다면, 전투 맵 쪽으로 세션에 접속한 모든 인원을 이동시킨다.
	if (bIsSuccess)
	{
		GetWorld()->ServerTravel("/Game/Maps/MainMap?Listen");
	}
	
}
