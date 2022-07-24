
#include "MGGGameInstance.h"
#include "OnlineSubsystem.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSessionSettings.h"


UMGGGameInstance::UMGGGameInstance()
{

}

void UMGGGameInstance::Init()
{
	if (IOnlineSubsystem* Ss = IOnlineSubsystem::Get())
	{
		SessionInterFaceSystem = Ss->GetSessionInterface();
		if (SessionInterFaceSystem.IsValid())
		{
			
			SessionInterFaceSystem->OnCreateSessionCompleteDelegates.AddUObject(this, &UMGGGameInstance::OnCreateSessionComplete);
			SessionInterFaceSystem->OnFindSessionsCompleteDelegates.AddUObject(this, &UMGGGameInstance::OnFindSessionComplete);
			SessionInterFaceSystem->OnJoinSessionCompleteDelegates.AddUObject(this, &UMGGGameInstance::OnJoinSessionComplete);
			OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &UMGGGameInstance::OnDestroySessionComplete);
			
		}
	}
}

void UMGGGameInstance::OnCreateSessionComplete(FName ServerName, bool Succeeded)
{
	if (Succeeded)
	{
		CreateServerDone = true;						
	}
}

void UMGGGameInstance::OnFindSessionComplete(bool Succeeded)
{
	TArray<FOnlineSessionSearchResult> Result;
	Result = SessionSearcher->SearchResults;
	FString ServerName;
	FOnlineSessionSearchResult OnResult;
	for (FOnlineSessionSearchResult Results : Result)
	{
		if (!Results.IsValid())
			continue;

		FServerInfo ServerInfo;

		Results.Session.SessionSettings.Get(FName("SERVER_NAME"), ServerName);
		ServerInfo.Servername = ServerName;
    
		for (int a=0; a < Result.Num(); ++a)
		{
			ServerInfo.CountID = a;
		}
    
		ServerInfo.MaximumPlayers= Results.Session.SessionSettings.NumPublicConnections;
		ServerInfo.CurrentPlayers= Results.Session.NumOpenPublicConnections;
		ServerListDel.Broadcast(ServerInfo);
		SessionSearcher->SearchResults;
	}
	
}

void UMGGGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type TResult)
{

	if (APlayerController* ThePlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		JoinAddressID;
			if (SessionInterFaceSystem->GetResolvedConnectString(SessionName, JoinAddressID))
			{
				ThePlayerController->ClientTravel(JoinAddressID, ETravelType::TRAVEL_Absolute);
			}
	}
}

void UMGGGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);
			if (bWasSuccessful)
			{
				UGameplayStatics::OpenLevel(GetWorld(), "MainMenu");
			}
		}
	}
}


void UMGGGameInstance::CreateServer(FString Servername)
{
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;
	if (IOnlineSubsystem::Get()->GetSubsystemName() != "NULL")
	{
		SessionSettings.bIsLANMatch = false;
	}
	else
	{
		SessionSettings.bIsLANMatch = true;
	}

	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.NumPublicConnections = 4;

	SessionSettings.Set(FName("Server_Name"), Servername, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	GetHereServerName = Servername;
	SessionInterFaceSystem->CreateSession(0, FName(Servername), SessionSettings);
}

void UMGGGameInstance::JoinServer()
{
	SessionSearcher = MakeShareable(new FOnlineSessionSearch());
  
	if (IOnlineSubsystem::Get()->GetSubsystemName() != "NULL")
	  {
		  SessionSearcher->bIsLanQuery = false;
	  }
  
	else
	  {
		  SessionSearcher->bIsLanQuery = true;
	  }
	SessionSearcher->MaxSearchResults = 10000;
	SessionSearcher->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	SessionInterFaceSystem->FindSessions(0, SessionSearcher.ToSharedRef());

}

void UMGGGameInstance::DestroySession(FName DestroyServerName)
{
	SessionInterFaceSystem->DestroySession(FName(DestroyServerName));
}

void UMGGGameInstance::TakeJoinGameInfo(FString SeasonName)
{
	GetHereServerName = SeasonName;
}

void UMGGGameInstance::JoinGame(int32 Value, FString SessionName)
{
	GetHereServerName = SessionName;
	SessionInterFaceSystem->JoinSession(0, FName(SessionName), SessionSearcher->SearchResults[Value]);
}

void UMGGGameInstance::ServerTraveler()
{
	if (HardHat == true)
	{
		GetWorld()->ServerTravel("/Game/FirstPersonCPP/Maps/HardHat?listen");
	}

	if (Crashzone == true)
	{
		GetWorld()->ServerTravel("/Game/MultiplayerMaps/Crashside?listen");
	}
}
