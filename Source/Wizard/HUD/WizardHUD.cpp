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

void AWizardHUD::SetCurrentDistrict(EDistrict District)
{
	WizardOverlay->SetCurrentDistrictText(UEnum::GetDisplayValueAsText<EDistrict>(District));
}

void AWizardHUD::SetEnergy(float Energy, float MaxEnergy)
{
	WizardOverlay->SetEnergyBarPercentage(Energy, MaxEnergy);
}

void AWizardHUD::SetPOIOnMiniMap(AActor* POIOwner)
{
	WizardOverlay->GetMiniMap()->AddPOI(POIOwner);
}

void AWizardHUD::SetStoreCatalog(TArray<FItemDataTable> Items)
{
	UCatalogWidget* Catalog = CreateWidget<UCatalogWidget>(GetOwningPlayerController(), WizardOverlay->GetCatalogWidgetClass());
	if (Catalog) {
		bool bCatalogCreated = Catalog->CreateCatalog(Items);
		if (bCatalogCreated && WizardOverlay->GetCenterBox()) {
			WizardOverlay->GetCenterBox()->AddChild(Catalog);
		}
	}
}

void AWizardHUD::AddCharacterItem(FItemDataTable Item)
{
	if (WizardOverlay->GetCharacterItemPanel()) {
		WizardOverlay->GetCharacterItemPanel()->AddCharacterItem(Item);
	}
}