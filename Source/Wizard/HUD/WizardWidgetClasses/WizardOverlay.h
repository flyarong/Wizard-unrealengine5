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
	/// Widget class for the District Panel
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Districts")
	TSubclassOf<class UUserWidget> DistrictPanelWidgetClass;

	/// <summary>
	/// Widget class for the Catalog
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Items")
	TSubclassOf<class UUserWidget> CatalogWidgetClass;

	/// <summary>
	/// Widget class for the Inventory
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Items")
	TSubclassOf<class UUserWidget> InventoryWidgetClass;

	/// <summary>
	/// Widget class for the local Message
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Messages")
	TSubclassOf<class UUserWidget> LocalMessageWidgetClass;

	/// <summary>
	/// Widget class for the public Message
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Messages")
	TSubclassOf<class UUserWidget> PublicMessageWidgetClass;

	/// <summary>
	/// Widget class for the SpellMap
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<class UUserWidget> SpellMapWidgetClass;

	/// <summary>
	/// Widget class for the Combat Menu
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<class UUserWidget> CombatMenuWidgetClass;

	/// <summary>
	/// Widget class for the Current Spell Step
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<class UUserWidget> CurrentStepWidgetClass;

	/// <summary>
	/// Widget class for the Current Spell Step Result
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<class UUserWidget> CurrentStepResultWidgetClass;

	/// <summary>
	/// Widget class for the Combat Score
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<class UUserWidget> CombatScoreWidgetClass;

	/// <summary>
	/// Widget class for the MatchState
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Match State")
	TSubclassOf<class UUserWidget> MatchStateWidgetClass;

	/// <summary>
	/// Widget used on the Overlay to show the current District
	/// </summary>
	UPROPERTY()
	class UDistrictPanelWidget* DistrictPanelWidget;

	/// <summary>
	/// Widget used on the Overlay to store the SpellMap
	/// </summary>
	UPROPERTY()
	class USpellMapWidget* SpellMapWidget;

	/// <summary>
	/// Widget used on the Overlay to store the Combat Score
	/// </summary>
	UPROPERTY()
	class UCombatScoreWidget* CombatScoreWidget;

	/// <summary>
	/// Widget used on the Overlay to show the Character's Inventory
	/// </summary>
	UPROPERTY()
	class UCharacterInventoryWidget* InventoryWidget;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CharacterNameText;

	UPROPERTY(meta = (BindWidget))
	class UImage* ProfileImage;

	UPROPERTY(meta = (BindWidget))
	class UButton* ProfileButton;

	UPROPERTY(meta = (BindWidget))
	UButton* InventoryButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SettingsButton;

	UPROPERTY(meta = (BindWidget))
	class UEndTurnButtonWidget* EndTurnButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* OffenseText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DefenseText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* WisdomText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* IntelligenceText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AgilityText;

	UPROPERTY(meta = (BindWidget))
	class UWizardProgressBarWidget* HealthBar;

	UPROPERTY(meta = (BindWidget))
	UWizardProgressBarWidget* PowerBar;

	UPROPERTY(meta = (BindWidget))
	UWizardProgressBarWidget* GoodProgressBar;

	UPROPERTY(meta = (BindWidget))
	UWizardProgressBarWidget* BadProgressBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* XPText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* GoodSpellText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DarkSpellText;

	UPROPERTY(meta = (BindWidget))
	class UMiniMapWidget* MiniMap;

	UPROPERTY(meta = (BindWidget))
	class UScaleBox* CenterBox;

	UPROPERTY(meta = (BindWidget))
	UScaleBox* TopCenterBox;

	UPROPERTY(meta = (BindWidget))
	UScaleBox* TopRightBox;

	UPROPERTY(meta = (BindWidget))
	UScaleBox* BottomBox;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* LocalEventBox;
	
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* PublicEventBox;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* LeftSideBox;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* RightSideBox;

	UPROPERTY(meta = (BindWidget))
	class UGridPanel* RightBottomPanel;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* LeftSideBoxFadeOut;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* LeftSideBoxFadeIn;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* RightSideBoxFadeOut;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* RightSideBoxFadeIn;

	UPROPERTY(meta = (BindWidget))
	class UChatBoxWidget* ChatBox;

public:
	FORCEINLINE TSubclassOf<UUserWidget> GetDistrictPanelWidgetClass() const { return DistrictPanelWidgetClass; }
	FORCEINLINE TSubclassOf<UUserWidget> GetCatalogWidgetClass() const { return CatalogWidgetClass; }
	FORCEINLINE TSubclassOf<UUserWidget> GetInventoryWidgetClass() const { return InventoryWidgetClass; }
	FORCEINLINE TSubclassOf<UUserWidget> GetLocalMessageWidgetClass() const { return LocalMessageWidgetClass; }
	FORCEINLINE TSubclassOf<UUserWidget> GetPublicMessageWidgetClass() const { return PublicMessageWidgetClass; }
	FORCEINLINE TSubclassOf<UUserWidget> GetSpellMapWidgetClass() const { return SpellMapWidgetClass; }
	FORCEINLINE TSubclassOf<UUserWidget> GetCombatMenuWidgetClass() const { return CombatMenuWidgetClass; }
	FORCEINLINE TSubclassOf<UUserWidget> GetCurrentStepWidgetClass() const { return CurrentStepWidgetClass; }
	FORCEINLINE TSubclassOf<UUserWidget> GetCurrentStepResultWidgetClass() const { return CurrentStepResultWidgetClass; }
	FORCEINLINE TSubclassOf<UUserWidget> GetCombatScoreWidgetClass() const { return CombatScoreWidgetClass; }
	FORCEINLINE TSubclassOf<UUserWidget> GetMatchStateWidgetClass() const { return MatchStateWidgetClass; }
	FORCEINLINE UDistrictPanelWidget* GetDistrictPanelWidget() const { return DistrictPanelWidget; }
	FORCEINLINE void SetDistrictPanelWidget(UDistrictPanelWidget* DistrictPanel) { DistrictPanelWidget = DistrictPanel; }
	FORCEINLINE USpellMapWidget* GetSpellMapWidget() const { return SpellMapWidget; }
	FORCEINLINE void SetSpellMapWidget(USpellMapWidget* Map) { SpellMapWidget = Map; }
	FORCEINLINE UCombatScoreWidget* GetCombatScoreWidget() const { return CombatScoreWidget; }
	FORCEINLINE void SetCombatScoreWidget(UCombatScoreWidget* CombatWidget) { CombatScoreWidget = CombatWidget; }
	FORCEINLINE UCharacterInventoryWidget* GetInventoryWidget() const { return InventoryWidget; }
	FORCEINLINE void SetInventoryWidget(UCharacterInventoryWidget* Inventory) { InventoryWidget = Inventory; }
	FORCEINLINE UImage* GetProfileImage() const { return ProfileImage; }
	FORCEINLINE UButton* GetProfileButton() const { return ProfileButton; }
	FORCEINLINE UButton* GetInventoryButton() const { return InventoryButton; }
	FORCEINLINE UButton* GetSettingsButton() const { return SettingsButton; }
	FORCEINLINE void SetProfileImage(UTexture2D* Image) { ProfileImage->SetBrushFromTexture(Image); }
	FORCEINLINE void SetHealthBarPercentage(float Health, float MaxHealth) { HealthBar->SetWizardBarPercent(Health, MaxHealth); };
	FORCEINLINE UWizardProgressBarWidget* GetHealthBar() const { return HealthBar; }
	FORCEINLINE void SetPowerBarPercentage(float Power, float MaxPower) { PowerBar->SetWizardBarPercent(Power, MaxPower); };
	FORCEINLINE UWizardProgressBarWidget* GetPowerBar() const { return PowerBar; }
	FORCEINLINE void SetGoodProgressBarPercentage(float GoodSpells) { GoodProgressBar->SetWizardBarPercent(GoodSpells, 100.f); };
	FORCEINLINE UWizardProgressBarWidget* GetGoodProgressBar() const { return GoodProgressBar; }
	FORCEINLINE void SetBadProgressBarPercentage(float DarkSpells) { BadProgressBar->SetWizardBarPercent(DarkSpells, 100.f); };
	FORCEINLINE UWizardProgressBarWidget* GetBadProgressBar() const { return BadProgressBar; }
	FORCEINLINE void SetXPText(FText XP) { XPText->SetText(XP); };
	FORCEINLINE UTextBlock* GetXPText() const { return XPText; }
	FORCEINLINE void SetGoodSpellText(FText GoodSpell) { GoodSpellText->SetText(GoodSpell); };
	FORCEINLINE UTextBlock* GetGoodSpellText() const { return GoodSpellText; }
	FORCEINLINE void SetDarkSpellText(FText DarkSpell) { DarkSpellText->SetText(DarkSpell); };
	FORCEINLINE UTextBlock* GetDarkSpellText() const { return DarkSpellText; }
	FORCEINLINE void SetCharacterNameText(FText Name) { CharacterNameText->SetText(Name); };
	FORCEINLINE UTextBlock* GetCharacterNameText() const { return CharacterNameText; }
	FORCEINLINE void SetOffenseText(FText Offense) { OffenseText->SetText(Offense); };
	FORCEINLINE UTextBlock* GetOffenseText() const { return OffenseText; }
	FORCEINLINE void SetDefenseText(FText Defense) { DefenseText->SetText(Defense); };
	FORCEINLINE UTextBlock* GetDefenseText() const { return DefenseText; }
	FORCEINLINE void SetWisdomText(FText Wisdom) { WisdomText->SetText(Wisdom); };
	FORCEINLINE UTextBlock* GetWisdomText() const { return WisdomText; }
	FORCEINLINE void SetIntelligenceText(FText Intelligence) { IntelligenceText->SetText(Intelligence); };
	FORCEINLINE UTextBlock* GetIntelligenceText() const { return IntelligenceText; }
	FORCEINLINE void SetAgilityText(FText Agility) { AgilityText->SetText(Agility); };
	FORCEINLINE UTextBlock* GetAgilityText() const { return AgilityText; }
	FORCEINLINE UMiniMapWidget* GetMiniMap() const { return MiniMap; }
	FORCEINLINE UEndTurnButtonWidget* GetEndTurnButton() const { return EndTurnButton; }
	FORCEINLINE UScaleBox* GetTopCenterBox() const { return TopCenterBox; }
	FORCEINLINE UScaleBox* GetCenterBox() const { return CenterBox; }
	FORCEINLINE UScaleBox* GetTopRightBox() const { return TopRightBox; }
	FORCEINLINE UScaleBox* GetBottomBox() const { return BottomBox; }
	FORCEINLINE UVerticalBox* GetLocalEventBox() const { return LocalEventBox; }
	FORCEINLINE UVerticalBox* GetPublicEventBox() const { return PublicEventBox; }
	FORCEINLINE UVerticalBox* GetLeftSideBox() const { return LeftSideBox; }
	FORCEINLINE UVerticalBox* GetRightSideBox() const { return RightSideBox; }
	FORCEINLINE UGridPanel* GetRightBottomPanel() const { return RightBottomPanel; }
	FORCEINLINE void PlayRightSideFadeInAnimation() { PlayAnimation(RightSideBoxFadeIn); }
	FORCEINLINE void PlayRightSideFadeOutAnimation() { PlayAnimation(RightSideBoxFadeOut); }
	FORCEINLINE void PlayLeftSideFadeInAnimation() { PlayAnimation(LeftSideBoxFadeIn); }
	FORCEINLINE void PlayLeftSideFadeOutAnimation() { PlayAnimation(LeftSideBoxFadeOut); }
	FORCEINLINE UChatBoxWidget* GetChatBox() const { return ChatBox; }
};
