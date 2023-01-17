// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Wizard/WizardTypes/ActionTypes.h"
#include "WizardGameMode.generated.h"

UCLASS(minimalapi)
class AWizardGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AWizardGameMode();
	virtual void Tick(float DeltaTime) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

protected:
	virtual void BeginPlay() override;

private:

	/// <summary>
	/// Game Instance pointer
	/// </summary>
	class UWizardGameInstance* WizardGameInstance;

	/// <summary>
	/// Function to initialize the joining player's character
	/// </summary>
	/// <param name="Controller">The player's controller</param>
	void InitCharacter(APlayerController* Controller);

public:

	/// <summary>
	/// Public getter for the player's character name
	/// </summary>
	/// <param name="PlayerName">Player's name</param>
	/// <returns>Character's name selected by the player</returns>
	FName GetPlayerCharacter(FString PlayerName);

	/// <summary>
	/// Function to get the cost of an action
	/// based on its type
	/// </summary>
	int32 GetActionCost(EAction Action);
};



