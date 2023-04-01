// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "WizardGameState.generated.h"

/**
 * GameState class to keep track of the
 * State of the Game
 */
UCLASS()
class WIZARD_API AWizardGameState : public AGameState
{
	GENERATED_BODY()
	
public:

	/// <summary>
	/// Function that moves all Enemies when
	/// the Game is in Enemy MatchState
	/// </summary>
	void MoveEnemies();

	/// <summary>
	/// Function to start the Trial
	/// for a Character
	/// </summary>
	/// <param name="Character">The Character who is about to face Trial</param>
	void StartTrial(class APawn* Character);

	/// <summary>
	/// Function to make WizardActors
	/// interactable before Turn starts
	/// </summary>
	void EnableWizardActors();

	/// <summary>
	/// Function to make WizardActors
	/// uninteractable after Turn ends
	/// </summary>
	void DisableWizardActors();

	/// <summary>
	/// Function to add a new Actor to the
	/// MiniMapActors array
	/// </summary>
	/// <param name="MiniMapActor">New MiniMap Actor</param>
	void AddMiniMapActor(class AActor* MiniMapActor);

	/// <summary>
	/// Function to remove an Actor from the
	/// MiniMapActors array
	/// </summary>
	/// <param name="MiniMapActor">Actor to remove</param>
	void RemoveMiniMapActor(AActor* MiniMapActor);

protected:
	virtual void BeginPlay() override;

private:

#pragma region WizardActors
	/// <summary>
	/// Array containing all the WizardActors
	/// present in the Game
	/// </summary>
	TArray<class IWizardActor*> WizardActors = TArray<IWizardActor*>();

	/// <summary>
	/// Number of Enemies who ended
	/// their turn
	/// </summary>
	UPROPERTY()
	int32 EnemiesFinished = 0;

	/// <summary>
	/// Function to add a WizardActor
	/// to the WizardActors Array
	/// </summary>
	/// <param name="WizardActor">the WizardActor</param>
	void AddWizardActor(AActor* WizardActor);

	/// <summary>
	/// Function to remove a WizardActor
	/// to the WizardActors Array
	/// </summary>
	/// <param name="CombatActor">the WizardActor</param>
	void RemoveWizardActor(AActor* WizardActor);

	/// <summary>
	/// Array containing all the WizardCombatActors
	/// present in the Game
	/// </summary>
	TArray<class IWizardCombatActor*> WizardCombatActors = TArray<IWizardCombatActor*>();

	/// <summary>
	/// Function to add a WizardCombatActor
	/// to the WizardCombatActors Array
	/// </summary>
	/// <param name="CombatActor">the Combat Actor</param>
	void AddCombatActor(AActor* CombatActor);

	/// <summary>
	/// Function to remove a WizardCombatActor
	/// to the WizardCombatActors Array
	/// </summary>
	/// <param name="CombatActor">the Combat Actor</param>
	void RemoveCombatActor(AActor* CombatActor);
#pragma endregion

	/// <summary>
	/// Array holding all the Actors that should be
	/// displayed on the MiniMap
	/// </summary>
	UPROPERTY()
	TArray<AActor*> MiniMapActors = TArray<AActor*>();

	/// <summary>
	/// Class to spawn a Trial Actor from
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Trials")
	TSubclassOf<class ATrial> TrialClass;

public:

	/// <summary>
	/// Get all the Actors present on the MiniMap
	/// </summary>
	/// <returns>Actors on MiniMap</returns>
	FORCEINLINE TArray<AActor*> GetMiniMapActors() const { return MiniMapActors; }
	FORCEINLINE void IncrementEnemies() { EnemiesFinished++; }
	FORCEINLINE bool EnemiesFinishedTheirTurn() const { return EnemiesFinished >= WizardCombatActors.Num(); }
};
