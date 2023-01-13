// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Wizard/WizardTypes/DistrictNames.h"
#include "TravelPopUpWidget.generated.h"

/**
 * Widget class for asking if the player wishes
 * to travel to a new district
 */
UCLASS()
class WIZARD_API UTravelPopUpWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void PopUpSetup(EDistrict District);
	void PopUpTearDown();

private:

	/// <summary>
	/// Text holding the new district's name
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* NewDistrictNameText;

	/// <summary>
	/// Button for travelling
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	class UButton* YesButton;

	/// <summary>
	/// Button to cancel travelling
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	class UButton* NoButton;

	/// <summary>
	/// Callback function to YesButton
	/// OnClicked event
	/// </summary>
	UFUNCTION()
	void YesButtonClicked();

	/// <summary>
	/// Callback function to NoButton
	/// OnClicked event
	/// </summary>
	UFUNCTION()
	void NoButtonClicked();

	/// <summary>
	/// District to move to
	/// </summary>
	UPROPERTY()
	EDistrict DistrictToMove;
};
