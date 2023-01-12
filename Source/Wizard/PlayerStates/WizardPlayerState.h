// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "WizardPlayerState.generated.h"

/**
 * Player State class for the Wizard
 */
UCLASS()
class WIZARD_API AWizardPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/// <summary>
	/// Function for setting the player's Character name
	/// </summary>
	UFUNCTION()
	void SetSelectedCharacter(FName RowName);

private:

	/// <summary>
	/// Replicated variable for initializing the player's 
	/// WizardCharacter
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_SelectedCharacter)
	FName CharacterName;

	/// <summary>
	/// RepNotify for CharacterName
	/// </summary>
	UFUNCTION()
	void OnRep_SelectedCharacter();

	/// <summary>
	/// Integer holding the number of actions a player
	/// can take per round
	/// </summary>
	UPROPERTY(Replicated, EditAnywhere, Category = "Wizard Gameplay")
	int32 NumOfActionsPerRound = 3;
};
