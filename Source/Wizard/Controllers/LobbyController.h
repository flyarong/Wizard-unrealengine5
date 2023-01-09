// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Wizard/HUD/LobbyHUD.h"
#include "LobbyController.generated.h"

/**
 * Player controller for the Lobby
 */
UCLASS()
class WIZARD_API ALobbyController : public APlayerController
{
	GENERATED_BODY()
	
public:

	/// <summary>
	/// Function setting up the Character selector menu
	/// </summary>
	void CharacterSelectorMenuSetup();

	/// <summary>
	/// Function for handling the selection of a character
	/// </summary>
	/// <param name="ButtonSelected">The character button that was clicked</param>
	UFUNCTION()
	void HandleCharacterSelection(class UCharacterButton* ButtonSelected);

	/// <summary>
	/// Function triggered when the Ready button is clicked
	/// </summary>
	UFUNCTION()
	void OnReadyButtonClicked();

	/// <summary>
	/// Function triggered when the Start Game button is clicked
	/// </summary>
	UFUNCTION()
	void OnStartGameButtonClicked();

	/// <summary>
	/// Function updating the character selector on the HUD
	/// </summary>
	void UpdateHUDCharacterSelector();

	/// <summary>
	/// Function updating the Player Ready List on the HUD
	/// </summary>
	/// <param name="BusyPlayers">Array of unready player names</param>
	void UpdateHUDPlayerList(TArray<FString> BusyPlayers);

protected:
	virtual void BeginPlay() override;

private:

#pragma region CharacterSelectorPointers
	/// <summary>
	/// HUD owned by this controller
	/// </summary>
	UPROPERTY()
	class ALobbyHUD* LobbyHUD;

	/// <summary>
	/// Character Selector GameMode pointer
	/// </summary>
	UPROPERTY()
	class ALobbyGameMode* LobbyGameMode;

	/// <summary>
	/// Character Selector GameState pointer
	/// </summary>
	UPROPERTY()
	class ALobbyGameState* LobbyGameState;

	/// <summary>
	/// PlayerStart assigned to the player for character selection
	/// </summary>
	UPROPERTY()
	class APlayerStart* SelectedPlayerStart;

	/// <summary>
	/// Array of PlayerStarts in the Character Selector
	/// </summary>
	UPROPERTY()
	TArray<class AActor*> PlayerStarts;
#pragma endregion

#pragma region CharacterSelection
	/// <summary>
	/// The currently selected character button
	/// </summary>
	UPROPERTY()
	int32 CurrentlySelectedButtonIndex = -1;

	UPROPERTY()
	FCharacterDataTable CurrentlySelectedCharacter;

	/// <summary>
	/// Server RPC to send the new selection to the server
	/// </summary>
	/// <param name="PreviousIndex">The index of the previous selection</param>
	/// <param name="NewIndex">The index of the new selection</param>
	UFUNCTION(Server, Reliable)
	void ServerSwitchButtonSelection(int32 PreviousIndex, int32 NewIndex, FName RowName, const FString& PlayerName);

	/// <summary>
	/// Function to save the newly selected character
	/// </summary>
	/// <param name="SelectedCharacterIndex">The selected character's index in the selection array</param>
	/// <param name="CharacterSelected">Struct holding information about the selected character</param>
	void SaveSelectedCharacterLocally(int32 SelectedCharacterIndex, const FCharacterDataTable& CharacterSelected);

	/// <summary>
	/// Server RPC to spawn character actor at a specific player start
	/// </summary>
	/// <param name="ChoosenPlayerStart">Selected player start</param>
	/// <param name="CurrentlySelectedCharacter">Previously selected actor which is to be destroyed</param>
	/// <param name="CharacterSelected">Newly selected character to spawn</param>
	/// <param name="PlayerName">Player's name to set in overhead widget</param>
	UFUNCTION(Server, Reliable)
	void ServerSpawnCharacterAtPlayerStart(
		APlayerStart* ChoosenPlayerStart,
		const FCharacterDataTable& CurrentSelection,
		const FCharacterDataTable& CharacterSelected,
		const FString& PlayerName
	);
#pragma endregion

#pragma region PlayerReadiness
	/// <summary>
	/// Server RPC to update the ready player list
	/// </summary>
	/// <param name="PlayerName">Player's name whom ready state has changed</param>
	/// <param name="bPlayerIsReady">What ready state the player is going into</param>
	UFUNCTION(Server, Reliable)
	void ServerUpdatePlayerList(const FString& PlayerName, bool bPlayerIsReady);

	/// <summary>
	/// Boolean holding the player's current readiness state
	/// </summary>
	bool bReady = false;
#pragma endregion
};
