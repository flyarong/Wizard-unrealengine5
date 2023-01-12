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
	void CreateWizardOverlay();

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

public:
	FORCEINLINE UWizardOverlay* GetOverlay() { return WizardOverlay; }
};
