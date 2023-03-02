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
#include "Wizard/Interfaces/PublicMessageActor.h"
#include "Wizard/WizardTypes/ActionTypes.h"
#include "Components/ScaleBox.h"
#include "Components/VerticalBox.h"
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
void AWizardHUD::ShowLeftPanel()
{
	if (WizardOverlay && WizardOverlay->GetLeftSideBox()) {
		UCanvasPanelSlot* Slot = UWidgetLayoutLibrary::SlotAsCanvasSlot(WizardOverlay->GetLeftSideBox());
		if (Slot) Slot->SetAlignment(FVector2D(0, 0));
	}
}

void AWizardHUD::HideLeftPanel()
{
	if (WizardOverlay && WizardOverlay->GetLeftSideBox()) {
		UCanvasPanelSlot* Slot = UWidgetLayoutLibrary::SlotAsCanvasSlot(WizardOverlay->GetLeftSideBox());
		if (Slot) Slot->SetAlignment(FVector2D(1, 0));
	}
}

void AWizardHUD::ShowRightPanel()
{
	if (WizardOverlay && WizardOverlay->GetRightSideBox()) {
		UCanvasPanelSlot* Slot = UWidgetLayoutLibrary::SlotAsCanvasSlot(WizardOverlay->GetRightSideBox());
		if (Slot) Slot->SetAlignment(FVector2D(1, 0));
	}
}

void AWizardHUD::HideRightPanel()
{
	if (WizardOverlay && WizardOverlay->GetRightSideBox()) {
		UCanvasPanelSlot* Slot = UWidgetLayoutLibrary::SlotAsCanvasSlot(WizardOverlay->GetRightSideBox());
		if (Slot) Slot->SetAlignment(FVector2D(0, 0));
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

void AWizardHUD::HideCurrentDistrict()
{
	if (WizardOverlay) WizardOverlay->PlayDistrictPanelFadeOut();
}
#pragma endregion

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
		WizardOverlay->GetInventoryButton()->OnClicked.AddDynamic(WController, &AWizardPlayerController::OpenHUDInventory);
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
