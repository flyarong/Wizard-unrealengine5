// Fill out your copyright notice in the Description page of Project Settings.


#include "WizardHUD.h"
#include "Wizard/HUD/WizardWidgetClasses/MiniMapWidget.h"
#include "Wizard/HUD/WizardWidgetClasses/CatalogWidget.h"
#include "Wizard/HUD/WizardWidgetClasses/CharacterItemPanelWidget.h"
#include "Wizard/HUD/WizardWidgetClasses/WizardOverlay.h"
#include "Components/ScaleBox.h"

bool AWizardHUD::CreateWizardOverlay()
{
	APlayerController* PlayerController = GetOwningPlayerController();
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

void AWizardHUD::SetEnergy(float Energy, float MaxEnergy)
{
	if (WizardOverlay && WizardOverlay->GetEnergyBar()) {
		WizardOverlay->SetEnergyBarPercentage(Energy, MaxEnergy);
	}
}

void AWizardHUD::SetCharacterImage(UTexture2D* CharacterImage)
{
	if (WizardOverlay && WizardOverlay->GetProfileImage()) {
		WizardOverlay->SetProfileImage(CharacterImage);
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

void AWizardHUD::SetCombat(int32 NewCombat)
{
	if (WizardOverlay && WizardOverlay->GetCombatText()) {
		WizardOverlay->SetCombatText(FText::AsNumber(NewCombat));
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
void AWizardHUD::SetStoreCatalog(TArray<FItemDataTable> Items)
{
	if (WizardOverlay) {
		if (WizardOverlay->GetCenterBox()->HasAnyChildren()) {
			WizardOverlay->GetCenterBox()->ClearChildren();
		}

		UCatalogWidget* Catalog = CreateWidget<UCatalogWidget>(GetOwningPlayerController(), WizardOverlay->GetCatalogWidgetClass());
		if (Catalog) {
			bool bCatalogCreated = Catalog->CreateCatalog(Items);
			if (bCatalogCreated && WizardOverlay->GetCenterBox()) {
				WizardOverlay->GetCenterBox()->AddChild(Catalog);
			}
		}
	}
}

void AWizardHUD::AddCharacterItem(FItemDataTable Item)
{
	if (WizardOverlay && WizardOverlay->GetCharacterItemPanel()) {
		WizardOverlay->GetCharacterItemPanel()->AddCharacterItem(Item);
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
