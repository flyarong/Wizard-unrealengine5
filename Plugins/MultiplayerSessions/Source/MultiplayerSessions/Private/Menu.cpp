// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu.h"
#include "Components/Button.h"
#include "Sound/SoundCue.h"
#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"

void UMenu::MenuSetup(int32 NumberOfPublicConnections, FString TypeOfMatch, FString LobbyPath)
{
	PathToLobby = FString::Printf(TEXT("%s?listen"), *LobbyPath);
	NumPublicConnections = NumberOfPublicConnections;
	MatchType = TypeOfMatch;

	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true; // setting the focus to this widget

	UWorld* World = GetWorld();
	if (World) {
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController) {
			// we are setting the input mode to ui only, so that way when our widget is created it is only focused on the ui (the menu), not applying focus on the pawns etc.
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget()); // widget we are supposed to be focusing on, takewidget() basically constructs this widget, so it satisfies the argument
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // we don't want to lock the mouse cursor in the menu
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true); // this is needed so we can actually see the mouse cursor
		}
	}

	// we can access it through the game instance, it gets created after the game instance is created
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance) {
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
	}

	// we are binding the callbacks to the custom subsystem delegates here, the delegate exists on the multiplayer session subsystem, so we have to check
	// if the multiplayer session subsystem is not null
	if (MultiplayerSessionsSubsystem) {
		MultiplayerSessionsSubsystem->MultiplayerOnCreateSessionComplete.AddDynamic(this, &ThisClass::OnCreateSession);
		// we are using adduobject for not dynamic delegates
		MultiplayerSessionsSubsystem->MultiplayerOnFindSessionsComplete.AddUObject(this, &ThisClass::OnFindSessions);
		MultiplayerSessionsSubsystem->MultiplayerOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSession);
		MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &ThisClass::OnDestroySession);
		MultiplayerSessionsSubsystem->MultiplayerOnStartSessionComplete.AddDynamic(this, &ThisClass::OnStartSession);
	}
}

// initialize is kind of like a constructor, so it is too early to setup properties to our menu widget here, that's why we have seperate setupwidget
bool UMenu::Initialize()
{
	if (!Super::Initialize()) return false;

	// we have delegates that exist on the button class, so we are binding our callbacks to those
	if (HostButton) {
		// onclicked is a dynamic multicast delegate, so we use adddynamic for binding functions to delegates
		HostButton->OnClicked.AddDynamic(this, &ThisClass::HostButtonClicked);
	}

	if (JoinButton) {
		JoinButton->OnClicked.AddDynamic(this, &ThisClass::JoinButtonClicked);
	}

	return true;
}

void UMenu::NativeDestruct()
{
	MenuTearDown();
	Super::NativeDestruct();
}

void UMenu::OnCreateSession(bool bWasSuccessful)
{
	if (bWasSuccessful) {
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("Session created successfully!")));
		}

		// ha a session létrejött, akkor a lobbyba akarjuk a szervert utaztatni, de meg akarjuk várni, amíg a session elkészül, ezért itt van ez a kód, és nem a hostbutton clickedben
		UWorld* World = GetWorld();
		if (World) {
			World->ServerTravel(PathToLobby); // with ?listen, we are opening the level as a listen server
		}
	}
	else {
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Failed to create session :(")));
		}

		// if bwassuccessful false, meaning the session failed to be created, then we enable the button again after we disabled it when we clicked it
		HostButton->SetIsEnabled(true);
	}
}

// this will be called after we broadcast our custom delegate MultiplayerSessionsSubsystem->MultiplayerOnFindSessionsComplete in UMultiplayerSessionsSubsystem::OnFindSessionsComplete
// because we bound this function to that delegate
// we can design how we want to handle finding session, but for know, we are just getting the first one from sessionresults which has the proper matchtype freeforall and join it
void UMenu::OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful)
{
	if (MultiplayerSessionsSubsystem == nullptr) return;

	for (auto Result : SessionResults) {
		// we can't use MatchType because menu.h has a private var called MatchType, hence SettingsValue
		FString SettingsValue;
		Result.Session.SessionSettings.Get(FName("MatchType"), SettingsValue);	// itt pedig kereshetjük a match type alapján, a SessionSettings.Get visszatér a MatchType változónkban egy értékkel, amennyiben talál az első paraméterben megadott kulcshoz tartozó valuet
		if (SettingsValue == MatchType) {
			// and if we found a matching session, we call the multiplayersubsystem's joinsession
			MultiplayerSessionsSubsystem->JoinSession(Result);
			// and after we call joinsession, there is no need to continue the loop or the function, so we return
			return;
		}
	}

	// if bwassuccessful false, or no one is hosting a session (sessionresults is empty), then we enable the join button which we disabled when clicked on it to find sessions
	if (!bWasSuccessful || SessionResults.Num() <= 0) {
		JoinButton->SetIsEnabled(true);
	}
}

// this is called when we broadcast our custom join session delegate
void UMenu::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
{
	// we want to get the correct address here, and travel to the address with clienttravel
	// we are accessing online session interface directly from here, but that is not breaking the one-way dependency, because online session interface does not depend on the menu
	// our menu can depend on the online session interface just like our subsystem as long as our subsystem does not depend on the menu neither
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem) {
		IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid()) {

			// itt tudjuk lekérni az IP címet a joinoláshoz, itt a connectinfoban (Address) lesz az IP cím, amivel ez a függvény visszatér a mi connect info változónkba
			// viszont ez a metódus alapvetően bool-al tér vissza, szóval tudjuk vizsgálni akár if-el hogy sikeres volt-e, de itt nem kell, mert ez a függvény amúgy is akkor hívódik csak meg, ha sikeres volt a join
			FString Address;
			SessionInterface->GetResolvedConnectString(NAME_GameSession, Address);

			// we have to use the game instance here to get the playercontroller because there is no getplayercontroller function in the menu class, we are not on a character or a pawn, so we need to get it from the game instance
			// miután a join megtörtént, jöhet az, hogy a lobbyba utazunk a klienssel, ahol a szerver is éppen van, ehhez a clienttravel kell, ami a playercontrollerben van
			// client travelnek kell a cím (address) hogy hova kell utaztatni a klienst, ez a szerver címe ugye, valamint a travel típusa
			APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
			if (PlayerController) {
				PlayerController->ClientTravel(Address, TRAVEL_Absolute);
			}
		}
	}

	// there is also the possibility of finding sessions, but we are not able to join them, so we need to re-enable the button here as well
	// in this case, the joinsession result is not success, so we need to check that
	if (Result != EOnJoinSessionCompleteResult::Success) {
		JoinButton->SetIsEnabled(true);
	}
}

// this is called when we broadcast whether the session was destroyed successfully or not from our custom multiplayer delegate
void UMenu::OnDestroySession(bool bWasSuccessful)
{
}

void UMenu::OnStartSession(bool bWasSuccessful)
{
}

void UMenu::HostButtonClicked()
{
	if (SessionClickSound) {
		PlaySound(SessionClickSound);
	}

	// as soon as we clicked this button (and join game button) we want to disable them so we don't call join session or create session
	HostButton->SetIsEnabled(false);
	if (MultiplayerSessionsSubsystem) {
		MultiplayerSessionsSubsystem->CreateSession(NumPublicConnections, MatchType);
	}
}

void UMenu::JoinButtonClicked()
{
	if (SessionClickSound) {
		PlaySound(SessionClickSound);
	}

	JoinButton->SetIsEnabled(false);
	if (MultiplayerSessionsSubsystem) {
		MultiplayerSessionsSubsystem->FindSessions(10000); // since we are using the steam dev id 480, there must a lot of sessions, so we give this a high number
	}
}

void UMenu::MenuTearDown()
{
	RemoveFromParent();
	UWorld* World = GetWorld();
	if (World) {
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController) {
			FInputModeGameOnly InputModeData; // gameonly input mode allows us to focus on the game, we can leave all of its settings on default
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(false); // and we hide the mouse cursor
		}
	}
}
