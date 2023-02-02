// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Wizard/MainMenu/MainMenu.h"
#include "LobbyMenu.generated.h"

/**
 * Menu widget class for the lobby/character selection
 * Uses main menu functionality for setup
 */
UCLASS()
class WIZARD_API ULobbyMenu : public UMainMenu
{
	GENERATED_BODY()

private:

	/// <summary>
	/// Character selector title text
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CharacterSelectorTitleText;

	/// <summary>
	/// Character selector grid panel
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* CharacterSelectorPanel;

	/// <summary>
	/// Grid panel to list the connected players
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* ConnectedPlayersPanel;

	/// <summary>
	/// Vertical box for placing the Ready button in
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* ReadyParent;

	/// <summary>
	/// Vertical box for placing the server's
	/// Start Game button in
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* StartGameParent;

	/// <summary>
	/// Vertical box containing the Character
	/// Selector Panel
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* CharacterSelectorBaseBox;

public:
	FORCEINLINE UTextBlock* GetCharacterSelectorTitleText() const { return CharacterSelectorTitleText; }
	FORCEINLINE UUniformGridPanel* GetCharacterSelectorPanel() const { return CharacterSelectorPanel; }
	FORCEINLINE UUniformGridPanel* GetConnectedPlayersPanel() const { return ConnectedPlayersPanel; }
	FORCEINLINE UVerticalBox* GetReadyParent() const { return ReadyParent; }
	FORCEINLINE UVerticalBox* GetStartGameParent() const { return StartGameParent; }
	FORCEINLINE UVerticalBox* GetCharacterSelectorBaseBox() const { return CharacterSelectorBaseBox; }
};
