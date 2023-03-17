// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WizardActor.h"
#include "Wizard/WizardTypes/CombatTypes.h"
#include "WizardCombatActor.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWizardCombatActor : public UWizardActor
{
	GENERATED_BODY()
};

/**
 * Interface for Actors to initiate Combat against
 */
class WIZARD_API IWizardCombatActor : public IWizardActor
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/// <summary>
	/// Function to damage the Actor after Combat
	/// </summary>
	virtual void ReceiveDamage(int32 Damage) = 0;

	/// <summary>
	/// Function to get the Actor's
	/// base damage used in Character defense
	/// Combat
	/// </summary>
	/// <returns>Actor's Base Damage</returns>
	virtual int32 GetBaseDamage();

	/// <summary>
	/// Function that gets the amount of damage to apply to
	/// a Character based on its Combat Score
	/// </summary>
	/// <param name="CharacterScore">Character's Combat Score</param>
	/// <returns>Damage Amount to apply on the Character</returns>
	virtual float GetDamage(int32 CharacterScore);

	/// <summary>
	/// Gets the Cost of Action for
	/// the Actor
	/// </summary>
	/// <returns>Action Cost</returns>
	virtual float GetCost() = 0;

	/// <summary>
	/// Gets the Health of the Actor
	/// </summary>
	/// <returns>Actor Health</returns>
	virtual int32 GetHealth() = 0;

	/// <summary>
	/// Function called when the Actor
	/// is defeated
	/// </summary>
	virtual void Kill() = 0;

	/// <summary>
	/// Function that gets the type of
	/// this Combat Actor
	/// </summary>
	/// <returns>Actor Combat Type</returns>
	virtual ECombat GetCombatType() = 0;

	/// <summary>
	/// Function to move the Combat Actor
	/// on the Map
	/// </summary>
	virtual void MoveCombatActor();
};
