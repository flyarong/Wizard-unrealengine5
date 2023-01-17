// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"
#include "Wizard/GameInstance/WizardGameInstance.h"
#include "Wizard/GameStates/LobbyGameState.h"
#include "Wizard/Controllers/LobbyController.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	LobbyGameState = LobbyGameState == nullptr ?
		Cast<ALobbyGameState>(GameState) : LobbyGameState;
	ALobbyController* NewController = Cast<ALobbyController>(NewPlayer);
	if (LobbyGameState && NewController) {
		NewController->ClientCharacterSelectorMenuSetup(LobbyGameState->SelectionStatus);
	}

	const FString PlayerName = NewPlayer->PlayerState->GetPlayerName();
	UpdatePlayerList(PlayerName);
}

void ALobbyGameMode::UpdatePlayerList(const FString& PlayerName, bool bIsReady)
{
	LobbyGameState = LobbyGameState == nullptr ?
		Cast<ALobbyGameState>(GameState) : LobbyGameState;
	if (LobbyGameState) {
		if (bIsReady) {
			LobbyGameState->UpdatePlayerList(FString(), PlayerName);
		}
		else {
			LobbyGameState->UpdatePlayerList(PlayerName);
		}
	}
}

void ALobbyGameMode::UpdateSelection(int32 PreviousIndex, int32 NewIndex, FName RowName, const FString& PlayerName)
{
	if (!PlayerName.IsEmpty()) {
		UWizardGameInstance* WizardGameInstance = Cast<UWizardGameInstance>(UGameplayStatics::GetGameInstance(this));
		if (WizardGameInstance) {
			WizardGameInstance->SelectedCharacters.Add(PlayerName, RowName);
		}
	}

	LobbyGameState = LobbyGameState == nullptr ?
		Cast<ALobbyGameState>(GameState) : LobbyGameState;
	if (LobbyGameState) {
		LobbyGameState->UpdateCharacterSelection(PreviousIndex, NewIndex);
	}
}

void ALobbyGameMode::StartGame()
{
	UWizardGameInstance* WizardGameInstance = Cast<UWizardGameInstance>(UGameplayStatics::GetGameInstance(this));
	FString GameMap;
	if (WizardGameInstance) {
		GameMap = WizardGameInstance->SelectedMap;
	}

	UWorld* World = GetWorld();
	if (!GameMap.IsEmpty() && World) {
		// bUseSeamlessTravel = true; // not working in PIE
		World->ServerTravel(FString::Printf(TEXT("%s?listen"), *GameMap));
	}
}