// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Wizard/HUD/WizardWidgetClasses/WizardProgressBarWidget.h"
#include "Components/TextBlock.h"
#include "WizardOverlay.generated.h"

/**
 * Overlay user widget class to show data
 * on the screen during game
 */
UCLASS()
class WIZARD_API UWizardOverlay : public UUserWidget
{
	GENERATED_BODY()
	
private:

	/// <summary>
	/// Widget class for the Catalog
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Catalog")
	TSubclassOf<class UUserWidget> CatalogWidgetClass;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentDistrictText;

	UPROPERTY(meta = (BindWidget))
	class UWizardProgressBarWidget* EnergyBar;

	UPROPERTY(meta = (BindWidget))
	class UMiniMapWidget* MiniMap;

	UPROPERTY(meta = (BindWidget))
	class UScaleBox* CenterBox;

public:
	FORCEINLINE TSubclassOf<UUserWidget> GetCatalogWidgetClass() const { return CatalogWidgetClass; }
	FORCEINLINE void SetEnergyBarPercentage(float Energy, float MaxEnergy) { EnergyBar->SetWizardBarPercent(Energy, MaxEnergy); };
	FORCEINLINE UWizardProgressBarWidget* GetEnergyBar() const { return EnergyBar; }
	FORCEINLINE void SetCurrentDistrictText(FText District) { CurrentDistrictText->SetText(District); };
	FORCEINLINE UTextBlock* GetCurrentDistrictText() const { return CurrentDistrictText; }
	FORCEINLINE UMiniMapWidget* GetMiniMap() const { return MiniMap; }
	FORCEINLINE UScaleBox* GetCenterBox() const { return CenterBox; }
};
