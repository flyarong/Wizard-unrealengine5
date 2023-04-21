// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

/**
 * Widget class for in-game menu
 */
UCLASS()
class WIZARD_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	/// <summary>
	/// Function to set the menu with the proper
	/// buttons to the screen
	/// </summary>
	/// <param name="bGameHasEnded">Whether the game has ended or not</param>
	void SetMenu(bool bGameHasEnded = false);

private:

	UPROPERTY(meta = (BindWidget))
	class UButton* ResumeButton;

	UPROPERTY(meta = (BindWidget))
	UButton* RestartButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ExitButton;

	UFUNCTION()
	void ResumeGame();

	UFUNCTION()
	void OnRestartButtonClicked();

	UFUNCTION()
	void OnExitButtonClicked();

	/// <summary>
	/// Multiplayer subsystem pointer
	/// </summary>
	UPROPERTY()
	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;

	/// <summary>
	/// Callback function to when the session is
	/// destroyed once the Player has left the game
	/// </summary>
	/// <param name="bWasSuccessful">Whether the session was destroyed successfully or not</param>
	UFUNCTION()
	void OnDestroySession(bool bWasSuccessful);

	/// <summary>
	/// Callback function to when the Player
	/// is leaving the game
	/// </summary>
	UFUNCTION()
	void OnPlayerLeftGame();
};
