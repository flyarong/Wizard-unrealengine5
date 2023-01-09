// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "LobbyGameState.generated.h"

/**
 * Lobby GameState class to track the current
 * status of the character selection
 */
UCLASS()
class WIZARD_API ALobbyGameState : public AGameState
{
	GENERATED_BODY()
	
public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

#pragma region PlayerList
	/// <summary>
	/// Function that updates the Player Ready List
	/// </summary>
	/// <param name="PlayerToAdd">Unready player</param>
	/// <param name="PlayerToRemove">Ready player</param>
	void UpdatePlayerList(FString PlayerToAdd = FString(), FString PlayerToRemove = FString());

	/// <summary>
	/// Array containing the unready player names
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_UpdatePlayerList)
	TArray<FString> BusyPlayers;

	/// <summary>
	/// RepNotify to replicate Player Ready List to the clients
	/// </summary>
	UFUNCTION()
	void OnRep_UpdatePlayerList();
#pragma endregion

#pragma region CharacterSelection
	/// <summary>
	/// Function that updates the current status of the character selection
	/// </summary>
	/// <param name="PreviousIndex">The previously selected character index</param>
	/// <param name="NewIndex">The newly selected character index</param>
	void UpdateCharacterSelection(int32 PreviousIndex, int32 NewIndex);

	/// <summary>
	/// Array of booleans holding the enabled status of each
	/// character
	/// </summary>
	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_UpdateCharacterSelection)
	TArray<bool> SelectionStatus;

	/// <summary>
	/// RepNotify to replicate the selection status to the clients
	/// </summary>
	UFUNCTION()
	void OnRep_UpdateCharacterSelection();
#pragma endregion
};
