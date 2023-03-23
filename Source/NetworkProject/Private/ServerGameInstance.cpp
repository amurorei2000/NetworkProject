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
		sessionSettings.Set(FName("KEY_RoomName"), roomName);

		// ������ �����Ѵ�.
		sessionInterface->CreateSession(0, sessionID, sessionSettings);

		UE_LOG(LogTemp, Warning, TEXT("Create Session try!"));
	}
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
