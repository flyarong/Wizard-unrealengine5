// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Wizard/WizardTypes/AttributeTypes.h"
#include "WizardGameMode.generated.h"

namespace MatchState
{
	extern WIZARD_API const FName Enemy;
	extern WIZARD_API const FName Trial;
	extern WIZARD_API const FName Prepare;
}

UCLASS(minimalapi)
class AWizardGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AWizardGameMode();
	virtual void Tick(float DeltaTime) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

	/// <summary>
	/// Function to Broadcast the Chat Message
	/// to all machines
	/// </summary>
	/// <param name="Message">The message to broadcast</param>
	void BroadcastChatMessage(const FText& Message);

	/// <summary>
	/// Function to broadcast the Public Message
	/// about a Character's victory in Combat
	/// </summary>
	/// <param name="WCharacter">The Character</param>
	/// <param name="CombatTarget">The Actor defeated in Combat</param>
	void BroadcastVictory(class AWizardCharacter* WCharacter, const TScriptInterface<class IWizardCombatActor>& CombatTarget);

	/// <summary>
	/// Function to broadcast the update of
	/// story points to all players
	/// </summary>
	/// <param name="NumOfPoints">Updated number of Story points</param>
	/// <param name="bIsPositivePoints">Whether or not it is an update in Positive Story points or not</param>
	void BroadcastStoryPointUpdate(const int32& NumOfPoints, bool bIsPositivePoints = true);

	/// <summary>
	/// Function to end the current game
	/// </summary>
	/// <param name="bIsGameWon">Whether the game has been won or not</param>
	void EndGame(bool bIsGameWon);

	/// <summary>
	/// Function to when a player is leaving
	/// the game
	/// </summary>
	/// <param name="WController">Player's Controller</param>
	void PlayerLeavingGame(class AWizardPlayerController* WController);

	/// <summary>
	/// Gets the Character with the lowest Attribute
	/// </summary>
	/// <param name="AttributeType">Attribute the min search is based on</param>
	/// <returns>Character with the lowest type of Attribute</returns>
	AWizardCharacter* GetCharacterWithLowestAttribute(EAttribute AttributeType);
	
	/// <summary>
	/// Function that gets the closest Wizard Character
	/// to an Enemy
	/// </summary>
	/// <param name="Enemy">Enemy to check the closest Character to</param>
	/// <returns>Character closest to the Enemy</returns>
	AWizardCharacter* GetClosestCharacter(class AActor* Enemy);

protected:
	virtual void BeginPlay() override;
	virtual void OnMatchStateSet() override;

private:

	/// <summary>
	/// Game Instance pointer
	/// </summary>
	class UWizardGameInstance* WizardGameInstance;

	/// <summary>
	/// Game State pointer
	/// </summary>
	class AWizardGameState* WizardGameState;

	/// <summary>
	/// Array of Game specific Player Controllers
	/// </summary>
	TArray<class AWizardPlayerController*> WizardPlayers;

	/// <summary>
	/// Boolean for whether or not the Players have been
	/// initialized
	/// </summary>
	UPROPERTY()
	bool bPlayersInitialized = false;

	/// <summary>
	/// Function to initialize the joining player's character
	/// </summary>
	/// <param name="Controller">The player's controller</param>
	void InitCharacter(AWizardPlayerController* Controller);

	/// <summary>
	/// Time since the Level has been started
	/// </summary>
	UPROPERTY()
	float LevelStartingTime = 0.f;

	/// <summary>
	/// Time to wait until start
	/// </summary>
	UPROPERTY()
	float WarmupTime = 5.f;

	/// <summary>
	/// Time of countdown
	/// </summary>
	UPROPERTY()
	float CountdownTime = 0.f;

	/// <summary>
	/// Number of Players who ended
	/// their turn
	/// </summary>
	UPROPERTY()
	int32 PlayersFinished = 0;

	/// <summary>
	/// Function to call when
	/// the Prepare state has finished
	/// </summary>
	UFUNCTION()
	void OnPrepareStateFinished();

	/// <summary>
	/// Callback function to when a
	/// threshold is reached with negative outcomes
	/// </summary>
	UFUNCTION()
	void OnThresholdReached();

public:

	FORCEINLINE TArray<class AWizardPlayerController*> GetWizardPlayers() const { return WizardPlayers; }

	/// <summary>
	/// Public getter for the player's character name
	/// </summary>
	/// <param name="PlayerName">Player's name</param>
	/// <returns>Character's name selected by the player</returns>
	FName GetPlayerCharacter(FString PlayerName);

	void IncrementPlayersFinished();
	void DecrementPlayersFinished();
	void IncrementEnemiesFinished();
};



