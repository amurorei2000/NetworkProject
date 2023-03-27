// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerGameInstance.h"
#include "OnlineSubSystem.h"
#include "OnlineSessionSettings.h"

// ������
UServerGameInstance::UServerGameInstance()
{
	sessionID = "Test Session";
}

// ���� ����Ǵ� �Լ�
void UServerGameInstance::Init()
{
	Super::Init();

	// �¶��� ���� ����� �����Ǿ� �ִ� IOnlineSubsystem Ŭ������ �����´�.
	IOnlineSubsystem* subsys = IOnlineSubsystem::Get();
	if (subsys)
	{
		sessionInterface = subsys->GetSessionInterface();
		
		if (sessionInterface != nullptr)
		{
			sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UServerGameInstance::OnCreateSessionComplete);
			sessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UServerGameInstance::OnFindSessionComplete);
			sessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UServerGameInstance::OnJoinSessionComplete);
		}

		// ���� �÷����� �̸��� ����Ѵ�.
		FString platformName = subsys->GetSubsystemName().ToString();
		UE_LOG(LogTemp, Warning, TEXT("Platform Name: %s"), *platformName);
	}
}

// ���� ���� �Լ�
void UServerGameInstance::CreateMySession(FString roomName, int32 playerCount)
{
	if (sessionInterface != nullptr)
	{
		// ���� ���� ������ �����.
		FOnlineSessionSettings sessionSettings;
		sessionSettings.bAllowInvites = false;
		sessionSettings.bAllowJoinInProgress = true;
		sessionSettings.bAllowJoinViaPresence = true;
		sessionSettings.bShouldAdvertise = true;
		sessionSettings.bIsDedicated = false;
		// �¶��� ����ý����� �̸��� "NULL"�̸� LAN ��Ī�� �ϰ�, "Steam"�̸� ���� ������ ��Ī�Ѵ�.
		sessionSettings.bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
		sessionSettings.NumPublicConnections = playerCount;
		sessionSettings.Set(FName("KEY_RoomName"), roomName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

		// ������ �����Ѵ�.
		sessionInterface->CreateSession(0, sessionID, sessionSettings);

		UE_LOG(LogTemp, Warning, TEXT("Create Session try!"));
	}
}

// ������� �ִ� ������ ã�� �Լ�
void UServerGameInstance::FindMySession()
{
	// ã������ ���� ������ �����Ѵ�.
	sessionSearch = MakeShareable(new FOnlineSessionSearch());

	sessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
	sessionSearch->MaxSearchResults = 30;
	// Ư�� Ű����� �˻� ������ ���͸��ϰ� ���� �� �߰��ϴ� ����
	//sessionSearch->QuerySettings.Set(SEARCH_KEYWORDS, FText("roomName"), EOnlineComparisonOp::GreaterThanEquals);

	// Presence�� ������ ������ ���͸��ϰ� ���� ��
	sessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	// �տ��� ���� ������ �̿��ؼ� ������ ã�´�.
	sessionInterface->FindSessions(0, sessionSearch.ToSharedRef());
}

// ã�� ���� ����Ʈ �߿��� Ư�� ���ǿ� ������ �� �� ����� �Լ�
void UServerGameInstance::JoinMySession(int32 sessionIdx)
{
	// �ε����� ������ �����ϰ� �� �������� �����Ѵ�.
	UE_LOG(LogTemp, Warning, TEXT("Room Number: %d"), sessionIdx);

	FOnlineSessionSearchResult selectedSession = sessionSearch->SearchResults[sessionIdx];
	sessionInterface->JoinSession(0, sessionID, selectedSession);
}

// ������ ������ ��������� �� ȣ��� �Լ�
void UServerGameInstance::OnCreateSessionComplete(FName sessionName, bool bIsSuccess)
{
	FString result = bIsSuccess ? TEXT("Create Session Success!") : TEXT("Create Session Failed...");
	UE_LOG(LogTemp, Warning, TEXT("%s : %s"), *result, *sessionName.ToString());

	// ���� ������ �����ߴٸ�, ���� �� ������ ���ǿ� ������ ��� �ο��� �̵���Ų��.
	if (bIsSuccess)
	{
		GetWorld()->ServerTravel("/Game/Maps/MainMap?Listen");
	}
	
}

// ���� �˻��� ������ �� ȣ��� �Լ�
void UServerGameInstance::OnFindSessionComplete(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		TArray<FOnlineSessionSearchResult> searchResults = sessionSearch->SearchResults;
		UE_LOG(LogTemp, Warning, TEXT("Find Session count: %d"), searchResults.Num());

		// �˻� ������� ��� ��ȸ�Ѵ�.
		for (int32 i = 0; i < searchResults.Num(); i++)
		{
			FSessionInfo searchedSessionInfo;

			FString roomName;
			searchResults[i].Session.SessionSettings.Get(FName("KEY_RoomName"), roomName);
			searchedSessionInfo.roomName = roomName;

			searchedSessionInfo.maxPlayers = searchResults[i].Session.SessionSettings.NumPublicConnections;

			searchedSessionInfo.currentPlayers = searchedSessionInfo.maxPlayers - searchResults[i].Session.NumOpenPublicConnections;

			searchedSessionInfo.ping = searchResults[i].PingInMs;
			searchedSessionInfo.idx = i;


			// ���� ������ �ʿ��� ������ �̺�Ʈ�� �����Ѵ�.
			searchResultDele.Broadcast(searchedSessionInfo);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Find Sessions Failed..."));
	}

	// ���ΰ�ħ ��ư�� �ٽ� Ȱ��ȭ�Ѵ�.
	searchFinishedDele.Broadcast();
}

// �ٸ� ���ǿ� �շ� ó���� ������ �� ȣ��Ǵ� �̺�Ʈ �Լ�
void UServerGameInstance::OnJoinSessionComplete(FName sessionName, EOnJoinSessionCompleteResult::Type joinResult)
{
	// ����, join�� �����ߴٸ� �ش� IP Address�� ���� �̵��� �Ѵ�.
	if (joinResult == EOnJoinSessionCompleteResult::Type::Success)
	{
		// ���� �̸����� IP �ּҸ� ȹ���Ѵ�.
		FString joinAddress;
		sessionInterface->GetResolvedConnectString(sessionName, joinAddress);

		UE_LOG(LogTemp, Warning, TEXT("Join Address: %s"), *joinAddress);

		if (APlayerController* pc = GetWorld()->GetFirstPlayerController())
		{
			pc->ClientTravel(joinAddress, ETravelType::TRAVEL_Absolute);
		}
		
	}

}
