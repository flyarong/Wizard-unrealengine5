// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Wizard/Wizard.h"
#include "Engine/GameInstance.h"
#include "WizardGameInstance.generated.h"

/**
 * Game Instance class for the game
 */
UCLASS()
class WIZARD_API UWizardGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	/// <summary>
	/// String storing path reference to the selected story/map
	/// </summary>
	UPROPERTY(BlueprintReadWrite)
	FString SelectedMap { DEFAULT_STORY_MAP };

	/// <summary>
	/// Struct storing RowName about the currently selected character
	/// for each player
	/// </summary>
	UPROPERTY()
	TMap<FString, FName> SelectedCharacters;

};
