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
	UTextBlock* OffenseText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* OffenseTextModifier;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DefenseText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DefenseTextModifier;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* WisdomText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* WisdomTextModifier;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* IntelligenceText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* IntelligenceTextModifier;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AgilityText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AgilityTextModifier;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentDistrictText;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* DistrictPanelFadeIn;

	UPROPERTY(meta = (BindWidget))
	class UWizardProgressBarWidget* HealthBar;

	UPROPERTY(meta = (BindWidget))
	class UWizardProgressBarWidget* PowerBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* XPText;

	UPROPERTY(meta = (BindWidget))
	class UMiniMapWidget* MiniMap;

	UPROPERTY(meta = (BindWidget))
	class UScaleBox* CenterBox;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* EventBox;

	UPROPERTY(meta = (BindWidget))
	class UCharacterItemPanelWidget* CharacterItemPanel;

public:
	FORCEINLINE TSubclassOf<UUserWidget> GetCatalogWidgetClass() const { return CatalogWidgetClass; }
	FORCEINLINE UImage* GetProfileImage() const { return ProfileImage; }
	FORCEINLINE void SetProfileImage(UTexture2D* Image) { ProfileImage->SetBrushFromTexture(Image); }
	FORCEINLINE void SetHealthBarPercentage(float Health, float MaxHealth) { HealthBar->SetWizardBarPercent(Health, MaxHealth); };
	FORCEINLINE UWizardProgressBarWidget* GetHealthBar() const { return HealthBar; }
	FORCEINLINE void SetPowerBarPercentage(float Power, float MaxPower) { PowerBar->SetWizardBarPercent(Power, MaxPower); };
	FORCEINLINE UWizardProgressBarWidget* GetPowerBar() const { return PowerBar; }
	FORCEINLINE void SetCurrentDistrictText(FText District) { CurrentDistrictText->SetText(District); };
	FORCEINLINE UTextBlock* GetCurrentDistrictText() const { return CurrentDistrictText; }
	FORCEINLINE void PlayDistrictPanelFadeIn() { PlayAnimation(DistrictPanelFadeIn); }
	FORCEINLINE void SetXPText(FText XP) { XPText->SetText(XP); };
	FORCEINLINE UTextBlock* GetXPText() const { return XPText; }
	FORCEINLINE void SetCharacterNameText(FText Name) { CharacterNameText->SetText(Name); };
	FORCEINLINE UTextBlock* GetCharacterNameText() const { return CharacterNameText; }
	FORCEINLINE void SetOffenseText(FText Offense) { OffenseText->SetText(Offense); };
	FORCEINLINE UTextBlock* GetOffenseText() const { return OffenseText; }
	FORCEINLINE void SetOffenseTextModifier(FText Modifier) { OffenseTextModifier->SetText(Modifier); };
	FORCEINLINE void SetDefenseText(FText Defense) { DefenseText->SetText(Defense); };
	FORCEINLINE UTextBlock* GetDefenseText() const { return DefenseText; }
	FORCEINLINE void SetDefenseTextModifier(FText Modifier) { DefenseTextModifier->SetText(Modifier); };
	FORCEINLINE void SetWisdomText(FText Wisdom) { WisdomText->SetText(Wisdom); };
	FORCEINLINE UTextBlock* GetWisdomText() const { return WisdomText; }
	FORCEINLINE void SetWisdomTextModifier(FText Modifier) { WisdomTextModifier->SetText(Modifier); };
	FORCEINLINE void SetIntelligenceText(FText Intelligence) { IntelligenceText->SetText(Intelligence); };
	FORCEINLINE UTextBlock* GetIntelligenceText() const { return IntelligenceText; }
	FORCEINLINE void SetIntelligenceTextModifier(FText Modifier) { IntelligenceTextModifier->SetText(Modifier); };
	FORCEINLINE void SetAgilityText(FText Agility) { AgilityText->SetText(Agility); };
	FORCEINLINE UTextBlock* GetAgilityText() const { return AgilityText; }
	FORCEINLINE void SetAgilityTextModifier(FText Modifier) { AgilityTextModifier->SetText(Modifier); };
	FORCEINLINE UMiniMapWidget* GetMiniMap() const { return MiniMap; }
	FORCEINLINE UScaleBox* GetCenterBox() const { return CenterBox; }
	FORCEINLINE UVerticalBox* GetEventBox() const { return EventBox; }
	FORCEINLINE UCharacterItemPanelWidget* GetCharacterItemPanel() const { return CharacterItemPanel; }
};
