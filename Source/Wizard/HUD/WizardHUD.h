// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Wizard/WizardTypes/DistrictNames.h"
#include "WizardHUD.generated.h"

/**
 * HUD class for the gameplay
 */
UCLASS()
class WIZARD_API AWizardHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	/// <summary>
	/// Create & add the overlay to the viewport
	/// </summary>
	/// <returns>Whether the creation was successful or not</returns>
	bool CreateWizardOverlay();

	void ShowTravelPopUp(EDistrict District);

	/// <summary>
	/// Function for printing the current district's name
	/// on the screen
	/// </summary>
	/// <param name="District">The current district to print</param>
	void SetCurrentDistrict(EDistrict District);

private:
	
	/// <summary>
	/// Widget Class of the gameplay overlay
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Wizard Overlay")
	TSubclassOf<class UUserWidget> WizardOverlayClass;

	/// <summary>
	/// Pointer to the gameplay overlay
	/// </summary>
	UPROPERTY()
	class UWizardOverlay* WizardOverlay;

	/// <summary>
	/// Widget Class of the gameplay overlay
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Wizard Overlay")
	TSubclassOf<class UUserWidget> TravelPopUpClass;

	/// <summary>
	/// Pointer to the gameplay overlay
	/// </summary>
	UPROPERTY()
	class UTravelPopUpWidget* TravelPopUp;

public:
	FORCEINLINE UWizardOverlay* GetOverlay() const { return WizardOverlay; }
};
