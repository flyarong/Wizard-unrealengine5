// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PublicMessageActor.h"
#include "WizardActor.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWizardActor : public UPublicMessageActor
{
	GENERATED_BODY()
};

/**
 * Interface for Actors that can be interacted
 * with in the game
 */
class WIZARD_API IWizardActor : public IPublicMessageActor
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/// <summary>
	/// Function to show/hide the Actor's Interact Widget
	/// </summary>
	/// <param name="bShowInteractWidget">Whether to show or hide the widget</param>
	virtual void ShowInteractWidget(bool bShowInteractWidget) = 0;

	/// <summary>
	/// Function to set whether or not the
	/// Actor can be interacted with
	/// </summary>
	virtual void SetCanInteract(bool bIsInteractable);
	virtual bool GetCanInteract();
};
