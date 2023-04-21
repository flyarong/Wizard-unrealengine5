// Fill out your copyright notice in the Description page of Project Settings.


#include "WizardHUD.h"
#include "Wizard/Controllers/WizardPlayerController.h"
#include "Wizard/HUD/WizardWidgetClasses/MiniMap/MiniMapWidget.h"
#include "Wizard/HUD/WizardWidgetClasses/Catalog/CatalogWidget.h"
#include "Wizard/HUD/WizardWidgetClasses/Items/CharacterInventoryWidget.h"
#include "Wizard/HUD/WizardWidgetClasses/WizardOverlay.h"
#include "Wizard/HUD/WizardWidgetClasses/Messages/LocalMessageWidget.h"
#include "Wizard/HUD/WizardWidgetClasses/Messages/PublicMessageWidget.h"
#include "Wizard/HUD/WizardWidgetClasses/Messages/ChatBoxWidget.h"
#include "Wizard/HUD/WizardWidgetClasses/Combat/SpellMapWidget.h"
#include "Wizard/HUD/WizardWidgetClasses/Combat/CombatMenuWidget.h"
#include "Wizard/HUD/WizardWidgetClasses/Combat/CurrentStepWidget.h"
#include "Wizard/HUD/WizardWidgetClasses/Combat/StepResultWidget.h"
#include "Wizard/HUD/WizardWidgetClasses/Combat/CombatScoreWidget.h"
#include "Wizard/HUD/WizardWidgetClasses/Districts/DistrictPanelWidget.h"
#include "Wizard/HUD/WizardWidgetClasses/EndTurnButtonWidget.h"
#include "Wizard/HUD/WizardWidgetClasses/MatchState/MatchStateWidget.h"
#include "Wizard/HUD/WizardWidgetClasses/Menu/MenuWidget.h"
#include "Wizard/HUD/WizardWidgetClasses/Menu/EndGameWidget.h"
#include "Wizard/Interfaces/PublicMessageActor.h"
#include "Wizard/WizardTypes/ActionTypes.h"
#include "Components/ScaleBox.h"
#include "Components/VerticalBox.h"
#include "Components/GridPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Button.h"
#include "Blueprint/WidgetLayoutLibrary.h"


bool AWizardHUD::CreateWizardOverlay()
{
	if (WizardOverlayClass)
	{
		WizardOverlay = CreateWidget<UWizardOverlay>(GetOwningPlayerController(), WizardOverlayClass);
		WizardOverlay->AddToViewport();
		
		return true;
	}

	return false;
}

#pragma region General
void AWizardHUD::ShowInGameMenu(bool bShowInGameMenu)
{
	ClearCenterBox();
	if (bShowInGameMenu && WizardOverlay && WizardOverlay->GetMenuWidgetClass() && WizardOverlay->GetCenterBox()) {
		UMenuWidget* InGameMenu = CreateWidget<UMenuWidget>(GetOwningPlayerController(), WizardOverlay->GetMenuWidgetClass());
		if (InGameMenu) {
			InGameMenu->SetMenu();
			WizardOverlay->GetCenterBox()->AddChild(InGameMenu);
		}
	}
}

void AWizardHUD::ShowEndGameMenu(bool bIsGameWon)
{
	ClearCenterBox();
	if (WizardOverlay && WizardOverlay->GetMenuWidgetClass() && WizardOverlay->GetEndGameWidgetClass() && WizardOverlay->GetCenterBox()) {
		UMenuWidget* InGameMenu = CreateWidget<UMenuWidget>(GetOwningPlayerController(), WizardOverlay->GetMenuWidgetClass());
		UEndGameWidget* EndGameMenu = CreateWidget<UEndGameWidget>(GetOwningPlayerController(), WizardOverlay->GetEndGameWidgetClass());
		if (EndGameMenu && InGameMenu) {
			EndGameMenu->SetEndGameScreen(bIsGameWon, InGameMenu);
			WizardOverlay->GetCenterBox()->AddChild(EndGameMenu);
		}
	}
}

void AWizardHUD::ShowLeftPanel()
{
	if (WizardOverlay && WizardOverlay->GetLeftSideBox()) {
		WizardOverlay->PlayLeftSideFadeInAnimation();
	}
}

void AWizardHUD::HideLeftPanel()
{
	if (WizardOverlay) {
		WizardOverlay->PlayLeftSideFadeOutAnimation();
	}
}

void AWizardHUD::ShowRightPanel()
{
	if (WizardOverlay) {
		if (WizardOverlay->GetEndTurnButton()) WizardOverlay->GetEndTurnButton()->SetIsButtonEnabled(true);
		if (WizardOverlay->GetProfileButton()) WizardOverlay->GetProfileButton()->SetIsEnabled(true);
		if (WizardOverlay->GetInventoryButton()) WizardOverlay->GetInventoryButton()->SetIsEnabled(true);
		if (WizardOverlay->GetSettingsButton()) WizardOverlay->GetSettingsButton()->SetIsEnabled(true);
		WizardOverlay->PlayRightSideFadeInAnimation();
	}
}

void AWizardHUD::HideRightPanel()
{
	if (WizardOverlay) {
		if (WizardOverlay->GetEndTurnButton()) WizardOverlay->GetEndTurnButton()->SetIsButtonEnabled(false);
		if (WizardOverlay->GetProfileButton()) WizardOverlay->GetProfileButton()->SetIsEnabled(false);
		if (WizardOverlay->GetInventoryButton()) WizardOverlay->GetInventoryButton()->SetIsEnabled(false);
		if (WizardOverlay->GetSettingsButton()) WizardOverlay->GetSettingsButton()->SetIsEnabled(false);
		WizardOverlay->PlayRightSideFadeOutAnimation();
	}
}

void AWizardHUD::ClearTopRightBox()
{
	if (WizardOverlay && WizardOverlay->GetTopRightBox()) {
		if (WizardOverlay->GetTopRightBox()->HasAnyChildren()) {
			WizardOverlay->GetTopRightBox()->ClearChildren();
		}
	}
}

void AWizardHUD::ClearTopCenterBox()
{
	if (WizardOverlay && WizardOverlay->GetTopCenterBox()) {
		if (WizardOverlay->GetTopCenterBox()->HasAnyChildren()) {
			WizardOverlay->GetTopCenterBox()->ClearChildren();
		}
	}
}

void AWizardHUD::ClearCenterBox()
{
	if (WizardOverlay && WizardOverlay->GetCenterBox()) {
		if (WizardOverlay->GetCenterBox()->HasAnyChildren()) {
			WizardOverlay->GetCenterBox()->ClearChildren();
		}
	}
}

void AWizardHUD::ClearBottomBox()
{
	if (WizardOverlay && WizardOverlay->GetBottomBox()) {
		if (WizardOverlay->GetBottomBox()->HasAnyChildren()) {
			WizardOverlay->GetBottomBox()->ClearChildren();
		}
	}
}
#pragma endregion

#pragma region Player
void AWizardHUD::SetCurrentDistrict(EDistrict District)
{
	if (WizardOverlay) {
		if (WizardOverlay->GetDistrictPanelWidget() == nullptr) {
			UDistrictPanelWidget* DistrictPanel = CreateWidget<UDistrictPanelWidget>(
				GetOwningPlayerController(), 
				WizardOverlay->GetDistrictPanelWidgetClass()
			);
			if (DistrictPanel) WizardOverlay->SetDistrictPanelWidget(DistrictPanel);
		}

		if (WizardOverlay->GetDistrictPanelWidget() && WizardOverlay->GetTopCenterBox()) {
			if (!WizardOverlay->GetTopCenterBox()->HasAnyChildren()) WizardOverlay->GetTopCenterBox()->AddChild(WizardOverlay->GetDistrictPanelWidget());
			WizardOverlay->GetDistrictPanelWidget()->SetCurrentDistrictText(UEnum::GetDisplayValueAsText<EDistrict>(District));
		}
	}
}

void AWizardHUD::SetHealth(float Health, float MaxHealth)
{
	if (WizardOverlay && WizardOverlay->GetHealthBar()) {
		WizardOverlay->SetHealthBarPercentage(Health, MaxHealth);
	}
}

void AWizardHUD::SetPower(float Power, float MaxPower)
{
	if (WizardOverlay && WizardOverlay->GetPowerBar()) {
		WizardOverlay->SetPowerBarPercentage(Power, MaxPower);
	}
}

void AWizardHUD::SetCharacterImage(UTexture2D* CharacterImage)
{
	AWizardPlayerController* WController = Cast<AWizardPlayerController>(GetOwningPlayerController());
	if (WController && WizardOverlay && WizardOverlay->GetProfileImage() && WizardOverlay->GetProfileButton() &&
		WizardOverlay->GetSettingsButton()) {
		WizardOverlay->SetProfileImage(CharacterImage);
		WizardOverlay->GetProfileButton()->OnClicked.AddDynamic(WController, &AWizardPlayerController::SetCameraFocusOnWizard);
		WizardOverlay->GetInventoryButton()->OnClicked.AddDynamic(WController, &AWizardPlayerController::OpenHUDInventory);
		WizardOverlay->GetSettingsButton()->OnClicked.AddDynamic(WController, &AWizardPlayerController::ToggleHUDInGameMenu);
	}
}

void AWizardHUD::SetCharacterName(FString CharacterName)
{
	if (WizardOverlay && WizardOverlay->GetCharacterNameText()) {
		WizardOverlay->SetCharacterNameText(FText::FromString(CharacterName));
	}
}

void AWizardHUD::SetXP(int32 NewXP)
{
	if (WizardOverlay && WizardOverlay->GetXPText()) {
		WizardOverlay->SetXPText(FText::AsNumber(NewXP));
	}
}

void AWizardHUD::SetOffense(int32 NewOffense)
{
	if (WizardOverlay && WizardOverlay->GetOffenseText()) {
		WizardOverlay->SetOffenseText(FText::AsNumber(NewOffense));
	}
}

void AWizardHUD::SetDefense(int32 NewDefense)
{
	if (WizardOverlay && WizardOverlay->GetDefenseText()) {
		WizardOverlay->SetDefenseText(FText::AsNumber(NewDefense));
	}
}

void AWizardHUD::SetWisdom(int32 NewWisdom)
{
	if (WizardOverlay && WizardOverlay->GetWisdomText()) {
		WizardOverlay->SetWisdomText(FText::AsNumber(NewWisdom));
	}
}

void AWizardHUD::SetIntelligence(int32 NewIntelligence)
{
	if (WizardOverlay && WizardOverlay->GetIntelligenceText()) {
		WizardOverlay->SetIntelligenceText(FText::AsNumber(NewIntelligence));
	}
}

void AWizardHUD::SetAgility(int32 NewAgility)
{
	if (WizardOverlay && WizardOverlay->GetAgilityText()) {
		WizardOverlay->SetAgilityText(FText::AsNumber(NewAgility));
	}
}

void AWizardHUD::SetSpells(int32 NewSpell, bool bIsGoodSpell)
{
	if (WizardOverlay) {
		FText SpellText = FText::FromString(FString::Printf(TEXT("%d"), NewSpell));
		if (bIsGoodSpell) {
			if (WizardOverlay->GetGoodSpellText()) WizardOverlay->SetGoodSpellText(SpellText);
		}
		else {
			if (WizardOverlay->GetDarkSpellText()) WizardOverlay->SetDarkSpellText(SpellText);
		}
	}
}
#pragma endregion

#pragma region Items
void AWizardHUD::SetStoreCatalog(AStore* Store)
{
	if (WizardOverlay && WizardOverlay->GetCenterBox()) {
		if (WizardOverlay->GetCenterBox()->HasAnyChildren()) {
			WizardOverlay->GetCenterBox()->ClearChildren();
		}

		UCatalogWidget* Catalog = CreateWidget<UCatalogWidget>(GetOwningPlayerController(), WizardOverlay->GetCatalogWidgetClass());
		if (Catalog) {
			bool bCatalogCreated = Catalog->CreateCatalog(Store);
			if (bCatalogCreated && WizardOverlay->GetCenterBox()) {
				WizardOverlay->GetCenterBox()->AddChild(Catalog);
			}
		}
	}
}

void AWizardHUD::UpdateCharacterInventory(const TArray<FItemDataTable>& Items)
{
	if (WizardOverlay) {
		if (WizardOverlay->GetInventoryWidget() == nullptr && WizardOverlay->GetInventoryWidgetClass()) {
			UCharacterInventoryWidget* InventoryWidget = CreateWidget<UCharacterInventoryWidget>(
					GetOwningPlayerController(),
					WizardOverlay->GetInventoryWidgetClass()
				);
			if (InventoryWidget) WizardOverlay->SetInventoryWidget(InventoryWidget);
		}
		
		if (WizardOverlay->GetInventoryWidget()) {
			WizardOverlay->GetInventoryWidget()->UpdateInventory(Items);
		}
	}
}
void AWizardHUD::ShowCharacterInventory()
{
	ClearCenterBox();
	if (WizardOverlay && WizardOverlay->GetCenterBox() && WizardOverlay->GetInventoryWidget()) {
		WizardOverlay->GetCenterBox()->AddChild(WizardOverlay->GetInventoryWidget());
	}
}
#pragma endregion

#pragma region MiniMap
void AWizardHUD::SetPOIOnMiniMap(AActor* POIOwner)
{
	if (WizardOverlay && WizardOverlay->GetMiniMap()) {
		WizardOverlay->GetMiniMap()->AddPOI(POIOwner);
	}
}
#pragma endregion

#pragma region Messages
void AWizardHUD::AddLocalMessage(const FString& Message, EAttribute AttributeType)
{
	if (WizardOverlay && WizardOverlay->GetLocalMessageWidgetClass()) {
		ULocalMessageWidget* LocalMessageWidget = CreateWidget<ULocalMessageWidget>(GetOwningPlayerController(), WizardOverlay->GetLocalMessageWidgetClass());
		if (LocalMessageWidget && WizardOverlay->GetLocalEventBox()) {
			LocalMessageWidget->AddLocalMessage(WizardOverlay->GetLocalEventBox(), FText::FromString(Message), AttributeType);
		}
	}
}

void AWizardHUD::AddPublicMessage(IPublicMessageActor* Source, EAction EventAction, IPublicMessageActor* Target)
{
	if (WizardOverlay && WizardOverlay->GetPublicMessageWidgetClass()) {
		UPublicMessageWidget* PublicMessageWidget = CreateWidget<UPublicMessageWidget>(
			GetOwningPlayerController(), 
			WizardOverlay->GetPublicMessageWidgetClass()
		);
		if (PublicMessageWidget && WizardOverlay->GetPublicEventBox()) {
			PublicMessageWidget->AddPublicMessage(
				WizardOverlay->GetPublicEventBox(), 
				Source->GetIcon(),
				EventAction,
				Target->GetIcon()
			);
		}
	}
}

void AWizardHUD::AddChatMessage(const FText& Message)
{
	if (WizardOverlay && WizardOverlay->GetChatBox()) {
		WizardOverlay->GetChatBox()->AddMessage(Message);
	}
}
#pragma endregion

#pragma region Combat
void AWizardHUD::CreateSpellMap(TMap<FKey, int32>& SpellMap)
{
	USpellMapWidget* SpellMapWidget = CreateWidget<USpellMapWidget>(GetOwningPlayerController(), WizardOverlay->GetSpellMapWidgetClass());
	if (SpellMapWidget && WizardOverlay) {
		SpellMapWidget->ConstructSpellMap(SpellMap);
		WizardOverlay->SetSpellMapWidget(SpellMapWidget);
	}
}

void AWizardHUD::AddSpellMap()
{
	if (WizardOverlay && WizardOverlay->GetTopRightBox()) {
		if (WizardOverlay->GetTopRightBox()->HasAnyChildren()) {
			WizardOverlay->GetTopRightBox()->ClearChildren();
		}

		WizardOverlay->GetTopRightBox()->AddChild(WizardOverlay->GetSpellMapWidget());
	}
}

void AWizardHUD::RemoveSpellMap()
{
	if (WizardOverlay) {
		ClearTopRightBox();
		WizardOverlay->SetSpellMapWidget(nullptr);
	}
}

void AWizardHUD::AddCombatMenu()
{
	if (WizardOverlay && WizardOverlay->GetCenterBox()) {
		if (WizardOverlay->GetCenterBox()->HasAnyChildren()) {
			WizardOverlay->GetCenterBox()->ClearChildren();
		}

		UCombatMenuWidget* CombatMenuWidget = CreateWidget<UCombatMenuWidget>(GetOwningPlayerController(), WizardOverlay->GetCombatMenuWidgetClass());
		if (CombatMenuWidget) {
			CombatMenuWidget->BindEventsToButtons();
			if (CombatMenuWidget->GetSpellMapBox() && WizardOverlay->GetSpellMapWidget()) {
				CombatMenuWidget->GetSpellMapBox()->ClearChildren();
				CombatMenuWidget->GetSpellMapBox()->AddChild(WizardOverlay->GetSpellMapWidget());
			}
			WizardOverlay->GetCenterBox()->AddChild(CombatMenuWidget);
		}
	}
}

void AWizardHUD::AddCurrentSpellStep(int32 CurrentSpellStep)
{
	if (WizardOverlay && WizardOverlay->GetCenterBox()) {
		if (WizardOverlay->GetCenterBox()->HasAnyChildren()) {
			WizardOverlay->GetCenterBox()->ClearChildren();
		}

		UCurrentStepWidget* CurrentStepWidget = CreateWidget<UCurrentStepWidget>(GetOwningPlayerController(), WizardOverlay->GetCurrentStepWidgetClass());
		if (CurrentStepWidget) {
			CurrentStepWidget->ConstructSpellStep(CurrentSpellStep);
			WizardOverlay->GetCenterBox()->AddChild(CurrentStepWidget);
		}
	}
}

void AWizardHUD::AddSpellStepResult(bool bWasSuccessful)
{
	if (WizardOverlay && WizardOverlay->GetCenterBox()) {
		if (WizardOverlay->GetCenterBox()->HasAnyChildren()) {
			WizardOverlay->GetCenterBox()->ClearChildren();
		}

		UStepResultWidget* StepResultWidget = CreateWidget<UStepResultWidget>(GetOwningPlayerController(), WizardOverlay->GetCurrentStepResultWidgetClass());
		if (StepResultWidget) {
			StepResultWidget->SetResult(bWasSuccessful);
			WizardOverlay->GetCenterBox()->AddChild(StepResultWidget);
		}
	}
}

void AWizardHUD::CreateCombatScore()
{
	if (WizardOverlay && WizardOverlay->GetCombatScoreWidgetClass()) {
		UCombatScoreWidget* CombatScoreWidget = CreateWidget<UCombatScoreWidget>(GetOwningPlayerController(), WizardOverlay->GetCombatScoreWidgetClass());
		if (CombatScoreWidget && WizardOverlay->GetBottomBox()) {
			WizardOverlay->SetCombatScoreWidget(CombatScoreWidget);
			if (WizardOverlay->GetBottomBox()->HasAnyChildren()) {
				WizardOverlay->GetBottomBox()->ClearChildren();
			}
			WizardOverlay->GetBottomBox()->AddChild(WizardOverlay->GetCombatScoreWidget());
		}
	}
}

void AWizardHUD::AddCombatScore(int32 Score)
{
	if (WizardOverlay && WizardOverlay->GetCombatScoreWidget()) {
		WizardOverlay->GetCombatScoreWidget()->SetCombatScore(FText::FromString(FString::Printf(TEXT("%d"), Score)));
	}
}
#pragma endregion

#pragma region MatchState
void AWizardHUD::AddMatchState(const FName& CurrentMatchState)
{
	ClearTopCenterBox();

	if (WizardOverlay && WizardOverlay->GetTopCenterBox() && WizardOverlay->GetMatchStateWidgetClass()) {
		UMatchStateWidget* MatchStateWidget = CreateWidget<UMatchStateWidget>(
			GetOwningPlayerController(),
			WizardOverlay->GetMatchStateWidgetClass()
		);
		if (MatchStateWidget) {
			WizardOverlay->GetTopCenterBox()->AddChild(MatchStateWidget);
			MatchStateWidget->SetMatchState(CurrentMatchState);
		}
	}
}

void AWizardHUD::SetPositiveStoryPoints(const float& NumOfPoints)
{
	if (WizardOverlay && WizardOverlay->GetGoodProgressBar()) {
		WizardOverlay->SetGoodProgressBarPercentage(NumOfPoints);
	}
}

void AWizardHUD::SetNegativeStoryPoints(const float& NumOfPoints)
{
	if (WizardOverlay && WizardOverlay->GetBadProgressBar()) {
		WizardOverlay->SetBadProgressBarPercentage(NumOfPoints);
	}
}
#pragma endregion