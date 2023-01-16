// Fill out your copyright notice in the Description page of Project Settings.


#include "WizardHUD.h"
#include "Wizard/HUD/WizardWidgetClasses/WizardOverlay.h"
#include "Wizard/HUD/WizardWidgetClasses/TravelPopUpWidget.h"

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

void AWizardHUD::ShowTravelPopUp(EDistrict District)
{
	if (TravelPopUpClass == nullptr) return;

	if (TravelPopUp == nullptr) {
		APlayerController* PlayerController = GetOwningPlayerController();
		if (PlayerController) TravelPopUp = CreateWidget<UTravelPopUpWidget>(PlayerController, TravelPopUpClass);
	}
	if (TravelPopUp) {
		TravelPopUp->PopUpSetup(District);
	}
}

void AWizardHUD::SetCurrentDistrict(EDistrict District)
{
	WizardOverlay->SetCurrentDistrictText(UEnum::GetDisplayValueAsText<EDistrict>(District));
}

void AWizardHUD::SetActions(int32 Actions)
{
	WizardOverlay->SetActionsText(FText::AsNumber(Actions));
}

void AWizardHUD::SetNumOfActions(int32 NumOfActions)
{
	WizardOverlay->SetNumOfActionsText(FText::AsNumber(NumOfActions));
}