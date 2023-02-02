// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyHUD.h"
#include "Wizard/Wizard.h"
#include "Components/VerticalBox.h"
#include "Components/UniformGridPanel.h"
#include "Wizard/Lobby/CharacterButton.h"
#include "Wizard/Lobby/LobbyMenu.h"
#include "Wizard/Lobby/ServerStartButton.h"
#include "Wizard/Lobby/ReadyButton.h"
#include "Wizard/Lobby/JoinedPlayer.h"
#include "Wizard/Controllers/LobbyController.h"

void ALobbyHUD::AddLobbyMenu() {
	OwningController = OwningController == nullptr ? Cast<ALobbyController>(GetOwningPlayerController()) : OwningController;
	if (OwningController && LobbyMenuClass) {
		LobbyMenu = CreateWidget<ULobbyMenu>(OwningController, LobbyMenuClass);
		LobbyMenu->MenuSetup();
	}
}

void ALobbyHUD::CreateCharacterButtons() {
	const FString CharacterSelectorTablePath{ CHARACTER_DATA_TABLE_PATH };
	UDataTable* CharacterSelectorTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *CharacterSelectorTablePath));

	if (CharacterSelectorTableObject) {
		int32 Index = 0;
		for (FName RowName : CharacterSelectorTableObject->GetRowNames()) {
			OwningController = Cast<ALobbyController>(GetOwningPlayerController());
			FCharacterDataTable* Row = CharacterSelectorTableObject->FindRow<FCharacterDataTable>(RowName, TEXT(""));
			UCharacterButton* CharacterButton = CreateWidget<UCharacterButton>(OwningController, CharacterButtonWidget);

			if (Row && CharacterButton) {
				// Setup the button
				CharacterButton->SetDataTableRowIndex(Index);
				CharacterButton->SetDataTableRowName(RowName);
				CharacterButton->SetCharacterNameText(Row->CharacterName);
				CharacterButton->SetCharacterStruct(*Row);

				// Bind to delegates
				CharacterButton->GetCharacterSelectButton()->OnClicked.AddDynamic(CharacterButton, &UCharacterButton::OnSelectButtonClicked);
				CharacterButton->CharacterSelectedDelegate.AddDynamic(OwningController, &ALobbyController::HandleCharacterSelection);

				CharacterButtons.Add(CharacterButton);
				Index++;
			}
		}
	}
}

void ALobbyHUD::SpawnReadyButton()
{
	OwningController = OwningController == nullptr ? Cast<ALobbyController>(GetOwningPlayerController()) : OwningController;
	if (OwningController && LobbyMenu && LobbyMenu->GetReadyParent()) {
		UReadyButton* ReadyButton = CreateWidget<UReadyButton>(OwningController, ReadyButtonWidget);
		if (ReadyButton) {
			ReadyButton->GetReadyButton();
			ReadyButton->GetReadyButton()->OnClicked.AddDynamic(OwningController, &ALobbyController::OnReadyButtonClicked);
			LobbyMenu->GetReadyParent()->AddChildToVerticalBox(ReadyButton);
		}
	}
}

void ALobbyHUD::SpawnStartGameButton(bool bIsButtonEnabled)
{
	OwningController = OwningController == nullptr ? Cast<ALobbyController>(GetOwningPlayerController()) : OwningController;
	if (OwningController && LobbyMenu && LobbyMenu->GetStartGameParent()) {
		if (LobbyMenu->GetStartGameParent()->HasAnyChildren()) {
			LobbyMenu->GetStartGameParent()->ClearChildren();
		}

		UServerStartButton* StartGameButton = CreateWidget<UServerStartButton>(OwningController, StartGameButtonWidget);
		if (StartGameButton) {
			StartGameButton->SetIsEnabled(bIsButtonEnabled);
			StartGameButton->GetStartGameButton()->OnClicked.AddDynamic(OwningController, &ALobbyController::OnStartGameButtonClicked);
			LobbyMenu->GetStartGameParent()->AddChildToVerticalBox(StartGameButton);
		}
	}
}

void ALobbyHUD::SpawnCharacterSelector(TArray<bool> SelectionStatus)
{
	bool bSelectorValid = CharacterButtons.Num() > 0 &&
		LobbyMenu &&
		LobbyMenu->GetCharacterSelectorPanel() &&
		LobbyMenu->GetCharacterSelectorTitleText();
	if (bSelectorValid) {
		LobbyMenu->GetCharacterSelectorTitleText()->SetText(FText::FromString(TEXT("Choose your character:")));

		for (int32 i = 0; i < CharacterButtons.Num(); i++) {
			if (CharacterButtons[i]) {
				if (SelectionStatus.Num() > 0) CharacterButtons[i]->SetIsEnabled(SelectionStatus[i]);
				LobbyMenu->GetCharacterSelectorPanel()->AddChildToUniformGrid(CharacterButtons[i], 0, i);
			}
		}
	}
}

void ALobbyHUD::RemoveCharacterSelector()
{
	bool bSelectorValid = LobbyMenu &&
		LobbyMenu->GetCharacterSelectorPanel() &&
		LobbyMenu->GetCharacterSelectorPanel()->HasAnyChildren();
	if (bSelectorValid) {
		LobbyMenu->GetCharacterSelectorPanel()->ClearChildren();
	}
}

void ALobbyHUD::SetReadyText()
{
	if (LobbyMenu && LobbyMenu->GetCharacterSelectorTitleText()) {
		LobbyMenu->GetCharacterSelectorTitleText()->SetText(FText::FromString(TEXT("Waiting for the other players...")));
	}
}

void ALobbyHUD::SpawnPlayerList(TArray<FString> BusyPlayers)
{
	if (LobbyMenu && LobbyMenu->GetConnectedPlayersPanel()) {

		// Player is not ready
		for (auto PlayerName : BusyPlayers) {
			if (!PlayerList.Contains(PlayerName)) PlayerList.Add(PlayerName, false);
		}

		OwningController = OwningController == nullptr ? Cast<ALobbyController>(GetOwningPlayerController()) : OwningController;
		TArray<UJoinedPlayer*> Players;
		if (OwningController) {
			for (auto Player : PlayerList) {

				// Player is ready
				if (!BusyPlayers.Contains(Player.Key)) Player.Value = true;

				// Create Player widget
				UJoinedPlayer* PlayerListElement = CreateWidget<UJoinedPlayer>(OwningController, PlayerListElementWidget);
				if (PlayerListElement) {
					PlayerListElement->SetJoinedPlayerText(Player.Key);
					if (PlayerReadyTexture && Player.Value) {
						PlayerListElement->SetReadyStatusImage(PlayerReadyTexture);
					}
					else if (PlayerNotReadyTexture && !Player.Value) {
						PlayerListElement->SetReadyStatusImage(PlayerNotReadyTexture);
					}

					Players.Add(PlayerListElement);
				}
			}
		}

		// (Re)Spawn Player List
		if (LobbyMenu->GetConnectedPlayersPanel()->HasAnyChildren()) {
			LobbyMenu->GetConnectedPlayersPanel()->ClearChildren();
		}
		int32 Row = 0;
		for (auto Element : Players) {
			LobbyMenu->GetConnectedPlayersPanel()->AddChildToUniformGrid(Element, Row, 0);
			Row++;
		}
	}
}