// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LobbyGameMode.generated.h"

/**
 * Lobby Game Mode class
 */
UCLASS()
class WIZARD_API ALobbyGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:

	virtual void PostLogin(APlayerController* NewPlayer) override;

	/// <summary>
	/// Function to trigger the GameState to track the current
	/// selection status
	/// </summary>
	/// <param name="PreviousIndex">Previously selected character index</param>
	/// <param name="NewIndex">Newly selected character index</param>
	void UpdateSelection(int32 PreviousIndex, int32 NewIndex, FName RowName, const FString& PlayerName);

	/// <summary>
	/// Function to trigger the GameState to track the current
	/// player readiness status
	/// </summary>
	/// <param name="PlayerName">Player's name</param>
	/// <param name="bIsReady">Player's new readiness state</param>
	void UpdatePlayerList(const FString& PlayerName, bool bIsReady = false);

	/// <summary>
	/// Function to start the game from the Character Selector
	/// </summary>
	void StartGame();

private:

	/// <summary>
	/// Lobby GameState pointer
	/// </summary>
	class ALobbyGameState* LobbyGameState;
};
