// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerState.h"
#include "Wizard/Lobby/CharacterButton.h"
#include "Wizard/Characters/WizardCharacter.h"
#include "Wizard/GameInstance/WizardGameInstance.h"
#include "Wizard/GameModes/LobbyGameMode.h"
#include "Wizard/GameStates/LobbyGameState.h"

void ALobbyController::BeginPlay()
{
	Super::BeginPlay();
}

void ALobbyController::ClientCharacterSelectorMenuSetup_Implementation(const TArray<bool>& SelectionStatus)
{
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);

	LobbyHUD = Cast<ALobbyHUD>(GetHUD());
	if (LobbyHUD) {
		LobbyHUD->CreateCharacterButtons();
		LobbyHUD->AddLobbyMenu();
		LobbyHUD->SpawnReadyButton();
		if (HasAuthority()) LobbyHUD->SpawnStartGameButton();
		LobbyHUD->SpawnCharacterSelector(SelectionStatus);
	}
}

void ALobbyController::HandleCharacterSelection(UCharacterButton* ButtonSelected)
{
	ServerSwitchButtonSelection(
		CurrentlySelectedButtonIndex, 
		ButtonSelected->GetDataTableRowIndex(), 
		ButtonSelected->GetDataTableRowName(),
		PlayerState->GetPlayerName()
	);
	SaveSelectedCharacterLocally(ButtonSelected->GetDataTableRowIndex(), ButtonSelected->GetCharacterStruct());
}

void ALobbyController::ServerSwitchButtonSelection_Implementation(int32 PreviousIndex, int32 NewIndex, FName RowName, const FString& PlayerName)
{
	LobbyGameMode = LobbyGameMode == nullptr ? 
		Cast<ALobbyGameMode>(UGameplayStatics::GetGameMode(this)) : LobbyGameMode;
	if (LobbyGameMode) {
		LobbyGameMode->UpdateSelection(PreviousIndex, NewIndex, RowName, PlayerName);
	}
}

void ALobbyController::UpdateHUDCharacterSelector(TArray<bool> SelectionStatus)
{
	if (bReady) return;

	LobbyHUD = LobbyHUD == nullptr ? Cast<ALobbyHUD>(GetHUD()) : LobbyHUD;

	if (LobbyHUD) {
		LobbyHUD->RemoveCharacterSelector();
		LobbyHUD->SpawnCharacterSelector(SelectionStatus);
	}
}

void ALobbyController::SaveSelectedCharacterLocally(int32 SelectedCharacterIndex, const FCharacterDataTable& CharacterSelected) {
	CurrentlySelectedButtonIndex = SelectedCharacterIndex;

	if (PlayerStarts.Num() > 0) {
		if (SelectedPlayerStart == nullptr) {
			SelectedPlayerStart = Cast<APlayerStart>(PlayerStarts[FMath::RandRange(0, PlayerStarts.Num() - 1)]);
		}
		ServerSpawnCharacterAtPlayerStart(SelectedPlayerStart, CurrentlySelectedCharacter, CharacterSelected, PlayerState->GetPlayerName());
		CurrentlySelectedCharacter = CharacterSelected;
	}
}

void ALobbyController::ServerSpawnCharacterAtPlayerStart_Implementation(APlayerStart* ChoosenPlayerStart, const FCharacterDataTable& CurrentSelection, const FCharacterDataTable& CharacterSelected, const FString& PlayerName) {
	UWorld* World = GetWorld();
	if (World && ChoosenPlayerStart && CharacterSelected.CharacterClass) {
		if (CurrentSelection.CharacterClass) {
			World->DestroyActor(Cast<AWizardCharacter>(UGameplayStatics::GetActorOfClass(this, CurrentSelection.CharacterClass)));
		}
		World->SpawnActor<AWizardCharacter>(
			CharacterSelected.CharacterClass,
			ChoosenPlayerStart->GetActorLocation(),
			ChoosenPlayerStart->GetActorRotation()
			);
	}
}

void ALobbyController::UpdateHUDPlayerList(TArray<FString> BusyPlayers)
{
	LobbyHUD = LobbyHUD == nullptr ? Cast<ALobbyHUD>(GetHUD()) : LobbyHUD;

	if (LobbyHUD) {
		// Players are ready
		if (BusyPlayers.Num() == 0 && HasAuthority()) LobbyHUD->SpawnStartGameButton(true);
		// Players are not ready
		if (BusyPlayers.Num() > 0 && HasAuthority()) LobbyHUD->SpawnStartGameButton(false);

		LobbyHUD->SpawnPlayerList(BusyPlayers);
	}
}

void ALobbyController::OnReadyButtonClicked() {
	// Character not selected
	if (CurrentlySelectedButtonIndex < 0) {
		return;
	}

	bReady = !bReady;
	LobbyHUD = LobbyHUD == nullptr ? Cast<ALobbyHUD>(GetHUD()) : LobbyHUD;
	LobbyGameState = LobbyGameState == nullptr ?
		Cast<ALobbyGameState>(UGameplayStatics::GetGameState(this)) : LobbyGameState;
	if (LobbyHUD && LobbyGameState) {
		if (bReady) {
			LobbyHUD->RemoveCharacterSelector();
		}
		else {
			LobbyHUD->SpawnCharacterSelector(LobbyGameState->SelectionStatus);
		}
	}

	ServerUpdatePlayerList(PlayerState->GetPlayerName(), bReady);
}

void ALobbyController::ServerUpdatePlayerList_Implementation(const FString& PlayerName, bool bPlayerIsReady) {
	LobbyGameMode = LobbyGameMode == nullptr ?
		Cast<ALobbyGameMode>(UGameplayStatics::GetGameMode(this)) : LobbyGameMode;
	if (LobbyGameMode) {
		LobbyGameMode->UpdatePlayerList(PlayerName, bPlayerIsReady);
	}
}

void ALobbyController::OnStartGameButtonClicked() {
	LobbyGameMode = LobbyGameMode == nullptr ?
		Cast<ALobbyGameMode>(UGameplayStatics::GetGameMode(this)) : LobbyGameMode;
	if (LobbyGameMode) {
		LobbyGameMode->StartGame();
	}
}