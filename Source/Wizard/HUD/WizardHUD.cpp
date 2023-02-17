// Fill out your copyright notice in the Description page of Project Settings.


#include "WizardHUD.h"
#include "Wizard/Controllers/WizardPlayerController.h"
#include "Wizard/HUD/WizardWidgetClasses/MiniMap/MiniMapWidget.h"
#include "Wizard/HUD/WizardWidgetClasses/Catalog/CatalogWidget.h"
#include "Wizard/HUD/WizardWidgetClasses/Items/CharacterItemPanelWidget.h"
#include "Wizard/HUD/WizardWidgetClasses/WizardOverlay.h"
#include "Wizard/HUD/WizardWidgetClasses/Messages/LocalMessageWidget.h"
#include "Wizard/HUD/WizardWidgetClasses/Messages/ChatBoxWidget.h"
#include "Wizard/HUD/WizardWidgetClasses/Combat/SpellMapWidget.h"
#include "Wizard/HUD/WizardWidgetClasses/Combat/CombatMenuWidget.h"
#include "Wizard/HUD/WizardWidgetClasses/Combat/CurrentStepWidget.h"
#include "Components/ScaleBox.h"
#include "Components/Button.h"

bool AWizardHUD::CreateWizardOverlay()
{
	PlayerController = GetOwningPlayerController();
	if (PlayerController && WizardOverlayClass)
	{
		WizardOverlay = CreateWidget<UWizardOverlay>(PlayerController, WizardOverlayClass);
		WizardOverlay->AddToViewport();
		
		return true;
	}

	return false;
}

#pragma region Player
void AWizardHUD::SetCurrentDistrict(EDistrict District)
{
	if (WizardOverlay && WizardOverlay->GetCurrentDistrictText()) {
		WizardOverlay->SetCurrentDistrictText(UEnum::GetDisplayValueAsText<EDistrict>(District));
		WizardOverlay->PlayDistrictPanelFadeIn();
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
	if (WController && WizardOverlay && WizardOverlay->GetProfileImage() && WizardOverlay->GetProfileButton()) {
		WizardOverlay->SetProfileImage(CharacterImage);
		WizardOverlay->GetProfileButton()->OnClicked.AddDynamic(WController, &AWizardPlayerController::SetCameraFocusOnWizard);
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
#pragma endregion

#pragma region Store/Catalog
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

void AWizardHUD::AddCharacterItem(int32 ItemIndex)
{
	if (WizardOverlay && WizardOverlay->GetCharacterItemPanel()) {
		WizardOverlay->GetCharacterItemPanel()->AddCharacterItem(ItemIndex);
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
	PlayerController = PlayerController == nullptr ? GetOwningPlayerController() : PlayerController;
	if (PlayerController) {
		ULocalMessageWidget* LocalMessageWidget = CreateWidget<ULocalMessageWidget>(PlayerController, LocalMessageWidgetClass);
		if (LocalMessageWidget && WizardOverlay) {
			LocalMessageWidget->AddLocalMessage(WizardOverlay, FText::FromString(Message), AttributeType);
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
	if (SpellMapWidget) {
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
		if (WizardOverlay->GetTopRightBox() && WizardOverlay->GetTopRightBox()->HasAnyChildren()) {
			WizardOverlay->GetTopRightBox()->ClearChildren();
		}
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
#pragma endregion
