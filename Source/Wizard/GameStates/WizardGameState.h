// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "WizardGameState.generated.h"

/// <summary>
/// Delegate type for broadcasting the end
/// of the Prepare MatchState
/// </summary>
DECLARE_DELEGATE(FOnPrepareFinished);

/// <summary>
/// Delegate type to broadcast when a
/// threshold is reached with negative outcomes
/// </summary>
DECLARE_DELEGATE(FOnThresholdReached);

/// <summary>
/// Delegate type to broadcast once DarkSpells
/// have been spawned in Prepare state
/// </summary>
DECLARE_MULTICAST_DELEGATE(FOnDarkSpellsSpawned);

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
	/// Delegate for when the Prepare MatchState
	/// has finished
	/// </summary>
	FOnPrepareFinished OnPrepareFinishedDelegate;

	/// <summary>
	/// Delegate for broadcasting when a threshold
	/// is reached with negative outcomes
	/// </summary>
	FOnThresholdReached OnThresholdReached;

	/// <summary>
	/// Delegate to broadcast after DarkSpells have been
	/// spawned during Prepare state
	/// </summary>
	FOnDarkSpellsSpawned OnDarkSpellsSpawnedDelegate;

	/// <summary>
	/// Function that moves all Enemies when
	/// the Game is in Enemy MatchState
	/// </summary>
	void MoveEnemies();

	/// <summary>
	/// Function to start the Trial
	/// for the Characters
	/// </summary>
	void StartTrial();

	/// <summary>
	/// Function to handle the preparation state
	/// of the turn
	/// </summary>
	void PrepareTurn();

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

	/// <summary>
	/// Function for handling a positive outcome
	/// </summary>
	void PositiveOutcome();

	/// <summary>
	/// Function for handling a negative outcome
	/// </summary>
	void NegativeOutcome();

protected:
	virtual void BeginPlay() override;

private:

#pragma region WizardActors
	
	/// <summary>
	/// GameMode pointer
	/// </summary>
	class AWizardGameMode* WGameMode;
	
	/// <summary>
	/// Array containing all the WizardActors
	/// present in the Game
	/// </summary>
	TArray<class IWizardActor*> WizardActors = TArray<IWizardActor*>();

	/// <summary>
	/// Array holding all the Actors that should be
	/// displayed on the MiniMap
	/// </summary>
	UPROPERTY()
	TArray<AActor*> MiniMapActors = TArray<AActor*>();

	/// <summary>
	/// Number of Enemies who ended
	/// their turn
	/// </summary>
	UPROPERTY()
	int32 EnemiesFinished = 0;

	/// <summary>
	/// Number of Good Spells present
	/// on the map
	/// </summary>
	UPROPERTY()
	int32 GoodSpells = 0;

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

#pragma region StoryPoints
	/// <summary>
	/// Number of story points received
	/// for positive outcomes
	/// </summary>
	UPROPERTY()
	int32 PositiveStoryPoints = 0;

	/// <summary>
	/// Number of story points received
	/// for negative outcomes
	/// </summary>
	UPROPERTY()
	int32 NegativeStoryPoints = 0;

	/// <summary>
	/// Number of negative story points until
	/// the next Threshold
	/// </summary>
	UPROPERTY()
	int32 ThresholdMeter = 0;

	/// <summary>
	/// Number of thresholds reached
	/// </summary>
	UPROPERTY()
	int32 NumOfThresholdsReached = 0;

	/// <summary>
	/// Function to check the current
	/// threshold and act accordingly
	/// </summary>
	void CheckThreshold();
#pragma endregion

#pragma region Enemy Spawning
	/// <summary>
	/// Array of Tags referring to CombatActors that
	/// may possibly be spawned at the Prepare state
	/// </summary>
	UPROPERTY()
	TArray<FName> Horde;

	/// <summary>
	/// Array of Tags used to pick a CombatActor
	/// to spawn
	/// </summary>
	UPROPERTY()
	TArray<FName> CachedHorde;

	/// <summary>
	/// Array of Spawn points to spawn Enemies
	/// on
	/// </summary>
	UPROPERTY()
	TArray<class AEnemySpawner*> EnemySpawnPoints = TArray<AEnemySpawner*>();

	/// <summary>
	/// Enemies to be spawned during Prepare state
	/// </summary>
	UPROPERTY()
	TArray<TSubclassOf<class AEnemy>> SpawnableEnemies = TArray<TSubclassOf<AEnemy>>();

	/// <summary>
	/// Spells to be spawned during Prepare state
	/// </summary>
	UPROPERTY()
	TArray<TSubclassOf<class ASpell>> SpawnableSpells = TArray<TSubclassOf<ASpell>>();

	/// <summary>
	/// Number of CombatActors to spawn per Player
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Spawnable Enemies")
	int32 NumOfActorsToSpawn = 2;

	/// <summary>
	/// Spell classes that can be spawned at Prepare state
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Spawnable Enemies")
	TArray<TSubclassOf<ASpell>> SpellClasses;

	/// <summary>
	/// Enemy classes that can be spawned at Prepare state
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Spawnable Enemies")
	TArray<TSubclassOf<AEnemy>> EnemyClasses;

	/// <summary>
	/// Elite Enemy classes that can be spawned
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Spawnable Enemies")
	TArray<TSubclassOf<AEnemy>> EliteEnemyClasses;

	/// <summary>
	/// Boss Enemy class of this Map
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Spawnable Enemies")
	TSubclassOf<AEnemy> BossClass;

	/// <summary>
	/// Function that spawns the selected Enemies
	/// and Spells
	/// </summary>
	void SpawnActors();

	/// <summary>
	/// Function to spawn an Enemy
	/// </summary>
	/// <param name="PawnClass">Class of the Enemy to spawn an instance of</param>
	void SpawnEnemy(TSubclassOf<AEnemy> EnemyClass);

	/// <summary>
	/// Function that sets all the Actors
	/// to be spawned in Prepare state
	/// </summary>
	void SetSpawnActorClasses();

	/// <summary>
	/// Function that adds the class of the Actor
	/// to spawn based on selected Tag from Horde
	/// </summary>
	/// <param name="Tag">The Tag selected</param>
	void AddSpawnActorClass(FName Tag);

	/// <summary>
	/// Returns a Vector containing the location
	/// of the chosen SpawnPoint
	/// </summary>
	FVector GetSpawnLocation();
#pragma endregion

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
	FORCEINLINE void AddEnemySpawnPoint(AEnemySpawner* SpawnPoint) { if (SpawnPoint) EnemySpawnPoints.Add(SpawnPoint); }
};
