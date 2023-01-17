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
	/// Character selector horizontal box
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* CharacterSelector;

	/// <summary>
	/// Vertical box to list the connected players
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* ConnectedPlayersBox;

	/// <summary>
	/// Horizontal box for placing the Ready button in
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* ReadyParent;

	/// <summary>
	/// Horizontal box for placing the server's
	/// Start Game button in
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* StartGameParent;

public:
	FORCEINLINE UTextBlock* GetCharacterSelectorTitleText() const { return CharacterSelectorTitleText; }
	FORCEINLINE UHorizontalBox* GetCharacterSelector() const { return CharacterSelector; }
	FORCEINLINE UVerticalBox* GetConnectedPlayersBox() const { return ConnectedPlayersBox; }
	FORCEINLINE UVerticalBox* GetReadyParent() const { return ReadyParent; }
	FORCEINLINE UVerticalBox* GetStartGameParent() const { return StartGameParent; }
};
