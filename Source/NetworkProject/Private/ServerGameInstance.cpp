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
		}
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
			FString roomName;
			searchResults[i].Session.SessionSettings.Get(FName("KEY_RoomName"), roomName);
			int32 maxPlayers = searchResults[i].Session.SessionSettings.NumPublicConnections;
			int32 currentPlayers = maxPlayers - searchResults[i].Session.NumOpenPublicConnections;
			int32 ping = searchResults[i].PingInMs;

			// ���� ������ �ʿ��� ������ �̺�Ʈ�� �����Ѵ�.
			searchResultDele.Broadcast(roomName, currentPlayers, maxPlayers, ping);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Find Sessions Failed..."));
	}
}
