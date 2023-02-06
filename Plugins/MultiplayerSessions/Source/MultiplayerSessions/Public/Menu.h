// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Menu.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BluePrintCallable)
	void MenuSetup(int32 NumberOfPublicConnections = 4, FString TypeOfMatch = FString(TEXT("FreeForAll")), FString LobbyPath = FString(TEXT("/Game/ThirdPerson/Maps/Lobby")));

protected:
	// this is an inherited function from uuserwidget, and it is a good place to bind our callback functions to the button delegates, it is protected function
	// so we are overriding it
	virtual bool Initialize() override;
	// this is another inherited function, it gets called when the level is removed from the world, that happens when we travel to another level, 
	// the current level gets destroyed, and all user widgets in that world call this function, so it is ideal for our menu tear down to be called inside this function
	// and prepare the game to be played in tp
	virtual void NativeDestruct() override;
	
	/**
	* Callbacks for the custom delegates on the Multiplayer sessions subsystem
	*/
	// we are making these protected, so if this menu class gets inherited, it is easier to override these callbacks
	// only the ones that are going to be bound to a dynamic multicast delegate will be ufunctions
	UFUNCTION()
	void OnCreateSession(bool bWasSuccessful);
	void OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);
	void OnJoinSession(EOnJoinSessionCompleteResult::Type Result);
	UFUNCTION()
	void OnDestroySession(bool bWasSuccessful);
	UFUNCTION()
	void OnStartSession(bool bWasSuccessful);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USoundCue* SelectSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USoundCue* SessionClickSound;

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton; // this is called forward declare, so we don't have to include the ubutton.h at the top
	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;

	// callback functions to button interactions, these are ufunctions because we are binding these to an onclicked event or delegate that exists in the ubutton class
	UFUNCTION()
	void HostButtonClicked();
	UFUNCTION()
	void JoinButtonClicked();

	// undo all the input changes done in the menu setup
	void MenuTearDown();

	// private variable for our multiplayer subsystem which we are forward declaring so we don't have to include it on top
	// we are instantiating it in the menu setup
	// Subsystem designed to handle all online session functionality
	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;

	int32 NumPublicConnections{4}; // we can use {} to initialize these with default values
	FString MatchType{ TEXT("FreeForAll") };
	FString PathToLobby{ TEXT("") };
};
