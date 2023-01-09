// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "MultiplayerSessionsSubsystem.generated.h"

/**
 * Declaring our own custom delegates for the Menu class to bind callbacks to
 */
// dynamic multicast delegate we broadcast from this class once we finished creating a new session, this way, we can make a callback on the menu that will be called in response
// we want it to have one parameter named bWasSuccessful so we can check whether the session creation was successful
// multicast means once its broadcasts multiple classes can their functions to it, dynamic means the delegate can be serialized and they can be saved or loaded from within
// a blueprint graph, in blueprints they are called event dispatchers
// any function we bind to a dynamic delegate HAS TO BE a ufunction
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnCreateSessionComplete, bool, bWasSuccessful);
// when we find sessions we are going to be broadcasting a delegate that our menu can have a callback for, and we pass in the TArray online sessions search results
// but online sessions search results type is not a uclass or a ustruct and if we want dynamic multicast delegates all the types that we are passing through them
// must be compatible with blueprints and online session search results is not so we cannot make this a dynamic delegate we must make it a multicast delegate
// search results will be a const reference type (const and &) that way we can pass in the array without having to copy it
// so as we discussed, because of FOnlineSessionSearchResult we cannot implement callbacks in blueprints for this because this is not a ustruct or uclass
// but if we would like to implement our whole menu class in blueprints and bind callbacks from blueprints that would be using event dispatchers then we could
// create a struct and make it a ustruct, and inherit FOnlineSessionSearchResult and pass that to a dynamic multicast delegate, but we are implementing our menu in c++
// so we shouldn't make this any more complex
DECLARE_MULTICAST_DELEGATE_TwoParams(FMultiplayerOnFindSessionsComplete, const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);
// EOnJoinSessionCompleteResult is also not compatible with blueprints
// also, in dynamic multicast delegate declaration we have a , between the type and name of the parameter, but in multicast delegate we don't
DECLARE_MULTICAST_DELEGATE_OneParam(FMultiplayerOnJoinSessionComplete, EOnJoinSessionCompleteResult::Type Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnDestroySessionComplete, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnStartSessionComplete, bool, bWasSuccessful);

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UMultiplayerSessionsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UMultiplayerSessionsSubsystem();

	/**
	 * To handle session functionality. The Menu class will call these.
	 */
	void CreateSession(int32 NumPublicConnections, FString MatchType);
	void FindSessions(int32 MaxSearchResults);
	void JoinSession(const FOnlineSessionSearchResult& SessionResult);
	void DestroySession();
	void StartSession();

	/**
	 * Our own custom delegates for the Menu class to bind callbacks to
	 */
	FMultiplayerOnCreateSessionComplete MultiplayerOnCreateSessionComplete;
	FMultiplayerOnFindSessionsComplete MultiplayerOnFindSessionsComplete;
	FMultiplayerOnJoinSessionComplete MultiplayerOnJoinSessionComplete;
	FMultiplayerOnDestroySessionComplete MultiplayerOnDestroySessionComplete;
	FMultiplayerOnStartSessionComplete MultiplayerOnStartSessionComplete;

protected:
	
	/**
	 * Internal callbacks for the delegates we'll add to the Online Session Interface delegate list
	 * These don't need to be called outside of this class.
	 */
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
	void OnStartSessionComplete(FName SessionName, bool bWasSuccessful);

private:
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSettings> LastSessionSettings; // settings of the lastly created session
	TSharedPtr<FOnlineSessionSearch> LastSessionSearch; // pointer for finding sessions

	/**
	 * To add to the Online Session Interface delegate list
	 * We'll bind our MultiplayerSessionsSubsystem internal callbacks to these.
	 */
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionCompleteDelegateHandle;
	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	FDelegateHandle FindSessionsCompleteDelegateHandle;
	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	FDelegateHandle JoinSessionCompleteDelegateHandle;
	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
	FDelegateHandle DestroySessionCompleteDelegateHandle;
	FOnStartSessionCompleteDelegate StartSessionCompleteDelegate;
	FDelegateHandle StartSessionCompleteDelegateHandle;

	// we are going to check this in our onsessiondestroyed callback and if its true, so a session is destroyed, we are going to create a new session
	bool bCreateSessionOnDestroy{ false };
	// if we know a session creation will fail because one already exists we want to store some information about this new session we'd like to create: the number of open public connections, and the matchtype
	// because these are the params needed for createsession function
	int32 LastNumPublicConnections;
	FString LastMatchType;
};
