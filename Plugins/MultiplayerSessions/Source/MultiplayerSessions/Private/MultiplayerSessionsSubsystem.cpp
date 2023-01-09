// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem():
	CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete)),
	FindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionsComplete)),
	JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionComplete)),
	DestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::OnDestroySessionComplete)),
	StartSessionCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartSessionComplete))
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem) {
		SessionInterface = Subsystem->GetSessionInterface();
	}
}

void UMultiplayerSessionsSubsystem::CreateSession(int32 NumPublicConnections, FString MatchType)
{
	if (!SessionInterface.IsValid()) return;

	auto ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);
	if (ExistingSession != nullptr) {
		// we have to make sure that the action of destroying a session has been completed before creating a session, otherwise we couldn't make a new session when exiting from the previous one
		// and we can use our custom destroy session function and delegates and callbacks for this
		bCreateSessionOnDestroy = true;
		LastNumPublicConnections = NumPublicConnections;
		LastMatchType = MatchType;
		DestroySession();
	}

	// Store the delegate in a delegate handle so we can remove it later from the delegate list
	CreateSessionCompleteDelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);

	LastSessionSettings = MakeShareable(new FOnlineSessionSettings());
	LastSessionSettings->bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL"; // if we use the null subsystem, then it is a lan match, otherwise it is not, and this will return NULL if we are using the null subsystem, and return steam, if we are using the steam subsystem
	LastSessionSettings->NumPublicConnections = NumPublicConnections;
	LastSessionSettings->bAllowJoinInProgress = true;
	LastSessionSettings->bAllowJoinViaPresence = true; // steam ún. presence-t használ, hogy a régiónkban lévő játékosokkal hozzon össze minket, ezt true-ra állítjuk a saját projektben is
	LastSessionSettings->bShouldAdvertise = true;  // ez engedélyezi a steam számára, hogy reklámozza ezt a sessiont, tehát mások is tudjanak csatlakozni
	LastSessionSettings->bUsesPresence = true; // ez pedig lehetővé teszi, hogy sessionöket találjunk a regionünkben
	LastSessionSettings->Set(FName("MatchType"), MatchType, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);	// a set function lehetőséget ad további tetszőleges settingek beállítására key-value párosok megadásával, mint mondjuk a match type: lehet hogy egy játékban mindenki mindenki ellen van, vagy csapat vs csapat stb., további megadhatjuk az advertisement typeot, hogy online servicen keresztül és ping alapján is megtalálható legyen
	LastSessionSettings->BuildUniqueId = 1; // with this set to 1, we can have multiple users launching their own builds and hosting and in that way when we search for valid game sessions we will see those other sessions and we will be able to join them otherwise we wouldn't be able to see them and we would be trying to join the first one that was hosted and if that game doesn't have any open connections then we won't be able to join it
	// we also want to make sure we can have as many players as we want, this is one other setting that needs to be added in one of the config files: DefaultGame.ini

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	// ha nem sikerült a session létrehozása, akkor töröljük a delegatet a delegate listből
	if (!SessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *LastSessionSettings)) {
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);

		// Broadcast our own custom delegate when we failed to create a session with the Broadcast method where we can pass in any values that we would like our callback function on the Menu class to receive
		// and we pass in false, because we haven't created the session successfully
		MultiplayerOnCreateSessionComplete.Broadcast(false);
	}
}

void UMultiplayerSessionsSubsystem::FindSessions(int32 MaxSearchResults)
{
	if (!SessionInterface.IsValid()) return;

	FindSessionsCompleteDelegateHandle = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);

	LastSessionSearch = MakeShareable(new FOnlineSessionSearch());
	LastSessionSearch->MaxSearchResults = MaxSearchResults;
	LastSessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL"; // we are setting this based on whether we are using the null subsystem or steam just like in createsession
	LastSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);	// we have to define this because we are using Presence, so when we call find sessions and we pass in our session search, we set the query settings to make sure that any sessions we find are using presence
	
	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	// find sessions returns a bool, so we can check if we found any sessions
	if (!SessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), LastSessionSearch.ToSharedRef())) {
		// if we don't find anything
		// 1. clear the delegate from the session interface delegate list
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);

		// 2. broadcast our custom delegate to the menu class, so it receives the information that find sessions was a failure
		// since the findsessions failed, we can pass in an empty tarray to fulfill the first parameter requirement
		MultiplayerOnFindSessionsComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
	}
}

void UMultiplayerSessionsSubsystem::JoinSession(const FOnlineSessionSearchResult& SessionResult)
{
	if (!SessionInterface.IsValid()) {
		// if the session interface is not valid, we broadcast to the menu that there is an unknown error when joining
		MultiplayerOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		return;
	}

	JoinSessionCompleteDelegateHandle = SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!SessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SessionResult)) {
		// join session has failed
		// 1. clear delegate from delegate list
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);

		// 2. broadcast to the menu class that something went wrong with the join
		MultiplayerOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
	}
}

// function for destroying a session if there is already an existing one
void UMultiplayerSessionsSubsystem::DestroySession()
{
	if (!SessionInterface.IsValid()) {
		// we are broadcasting that the destroy wasn't successfuly is the session interface isn't valid
		MultiplayerOnDestroySessionComplete.Broadcast(false);
		return;
	}
	
	DestroySessionCompleteDelegateHandle = SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegate);

	// this returns a bool so we can check this if it was successful
	if (!SessionInterface->DestroySession(NAME_GameSession)) {
		// in this, destroying the session has failed
		// 1. clear delegate
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);

		// 2. broadcast false because the destroy failed
		MultiplayerOnDestroySessionComplete.Broadcast(false);
	}
}

void UMultiplayerSessionsSubsystem::StartSession()
{
}

// these are fired off when the sessioninterface loops through its delegate list and fires off these callbacks
// so at this point, the action of creating a session (UMultiplayerSessionsSubsystem::CreateSession method completed) is complete
void UMultiplayerSessionsSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	// 1. we finish creating a session so we can remove the delegate from the online session interface delegate list
	if (SessionInterface) {
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
	}

	// 2. we broadcast our custom delegate so our menu class can have its callback function called
	// we pass in bWasSuccessful, because that parameter holds the value whether the session creation was successful, we would expect this to be true
	MultiplayerOnCreateSessionComplete.Broadcast(bWasSuccessful);
}

// this callback is called when UMultiplayerSessionsSubsystem::FindSessions was complete, so we expect it to be successful
void UMultiplayerSessionsSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{
	if (SessionInterface) {
		// clear delegate from delegate list
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
	}

	// in the event of getting some search results but the lastsessionsearch results is empty, because we haven't found any results relevant to us (matchtype = free for all)
	// we would like to broadcast, so that the menu knows we haven't found any valid search results
	if (LastSessionSearch->SearchResults.Num() <= 0) {
		MultiplayerOnFindSessionsComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		// and we return, because we don't want the last broadcast to be run
		return;
	}

	// broadcast delegate so our menu class can have its callback function called
	MultiplayerOnFindSessionsComplete.Broadcast(LastSessionSearch->SearchResults, bWasSuccessful);
}

// this is called when joining a session is succeeded
void UMultiplayerSessionsSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	// 1. clear delegate
	if (SessionInterface) {
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
	}

	// 2. broadcast to the menu to inform it that we joined the session
	MultiplayerOnJoinSessionComplete.Broadcast(Result);
}

// this is called when destroying a session supposed to be succeeded
void UMultiplayerSessionsSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (SessionInterface) {
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
	}

	// we need to check if we are going to create a new session or not, because in the event of creating a new session while there is an existing one, first we destroy then we create
	// we only do this, if the destroy was successful, and our boolean is true, meaning we need to create a session after we destroyed the previous one
	if (bWasSuccessful && bCreateSessionOnDestroy) {
		// we set this boolean to its default value, which was false, so at the next destroy, we don't accidentally create a session if we don't need to
		bCreateSessionOnDestroy = false;
		CreateSession(LastNumPublicConnections, LastMatchType);
	}

	// regardless of whether we created a new session or not, we have to broadcast to the menu class that the session has been destroyed successfully so we can
	// do something in that case with the menu and such
	MultiplayerOnDestroySessionComplete.Broadcast(bWasSuccessful);
}

void UMultiplayerSessionsSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
}
