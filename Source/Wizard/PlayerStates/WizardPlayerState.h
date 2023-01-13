// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Wizard/WizardTypes/ActionTypes.h"
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

	/// <summary>
	/// Function to handle the number of actions
	/// for the player
	/// </summary>
	/// <param name="Action">The action which is executed</param>
	UFUNCTION()
	void SpendAction(EAction Action);

private:

	/// <summary>
	/// Player controller pointer
	/// </summary>
	class AWizardPlayerController* Controller;

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
	/// Integer holding the number of actions
	/// the player currently has
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_Actions)
	int32 Actions = 3;

	UFUNCTION()
	void OnRep_Actions();

	/// <summary>
	/// Integer holding the number of actions a player
	/// can take per round
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Wizard Gameplay")
	int32 NumOfActionsPerRound = 3;
	
	/// <summary>
	/// Variable storing the cost of the current
	/// action
	/// </summary>
	int32 CostOfAction = 0;

	/// <summary>
	/// Server RPC for calculating the cost of the
	/// action based on its type
	/// </summary>
	UFUNCTION(Server, Reliable)
	void ServerCalculateAction(EAction Action);
};
