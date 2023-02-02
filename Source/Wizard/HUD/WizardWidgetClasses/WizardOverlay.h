// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Wizard/HUD/WizardWidgetClasses/WizardProgressBarWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
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
	class UTextBlock* CharacterNameText;

	UPROPERTY(meta = (BindWidget))
	class UImage* ProfileImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CombatText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* WisdomText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* IntelligenceText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AgilityText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentDistrictText;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* DistrictPanelFadeIn;

	UPROPERTY(meta = (BindWidget))
	class UWizardProgressBarWidget* EnergyBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* XPText;

	UPROPERTY(meta = (BindWidget))
	class UMiniMapWidget* MiniMap;

	UPROPERTY(meta = (BindWidget))
	class UScaleBox* CenterBox;

	UPROPERTY(meta = (BindWidget))
	class UCharacterItemPanelWidget* CharacterItemPanel;

public:
	FORCEINLINE TSubclassOf<UUserWidget> GetCatalogWidgetClass() const { return CatalogWidgetClass; }
	FORCEINLINE UImage* GetProfileImage() const { return ProfileImage; }
	FORCEINLINE void SetProfileImage(UTexture2D* Image) { ProfileImage->SetBrushFromTexture(Image); }
	FORCEINLINE void SetEnergyBarPercentage(float Energy, float MaxEnergy) { EnergyBar->SetWizardBarPercent(Energy, MaxEnergy); };
	FORCEINLINE UWizardProgressBarWidget* GetEnergyBar() const { return EnergyBar; }
	FORCEINLINE void SetCurrentDistrictText(FText District) { CurrentDistrictText->SetText(District); };
	FORCEINLINE UTextBlock* GetCurrentDistrictText() const { return CurrentDistrictText; }
	FORCEINLINE void PlayDistrictPanelFadeIn() { PlayAnimation(DistrictPanelFadeIn); }
	FORCEINLINE void SetXPText(FText XP) { XPText->SetText(XP); };
	FORCEINLINE UTextBlock* GetXPText() const { return XPText; }
	FORCEINLINE void SetCharacterNameText(FText Name) { CharacterNameText->SetText(Name); };
	FORCEINLINE UTextBlock* GetCharacterNameText() const { return CharacterNameText; }
	FORCEINLINE void SetCombatText(FText Combat) { CombatText->SetText(Combat); };
	FORCEINLINE UTextBlock* GetCombatText() const { return CombatText; }
	FORCEINLINE void SetWisdomText(FText Wisdom) { WisdomText->SetText(Wisdom); };
	FORCEINLINE UTextBlock* GetWisdomText() const { return WisdomText; }
	FORCEINLINE void SetIntelligenceText(FText Intelligence) { IntelligenceText->SetText(Intelligence); };
	FORCEINLINE UTextBlock* GetIntelligenceText() const { return IntelligenceText; }
	FORCEINLINE void SetAgilityText(FText Agility) { AgilityText->SetText(Agility); };
	FORCEINLINE UTextBlock* GetAgilityText() const { return AgilityText; }
	FORCEINLINE UMiniMapWidget* GetMiniMap() const { return MiniMap; }
	FORCEINLINE UScaleBox* GetCenterBox() const { return CenterBox; }
	FORCEINLINE UCharacterItemPanelWidget* GetCharacterItemPanel() const { return CharacterItemPanel; }
};
