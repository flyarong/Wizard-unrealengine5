// Fill out your copyright notice in the Description page of Project Settings.


#include "WizardHUD.h"
#include "Wizard/HUD/WizardWidgetClasses/WizardOverlay.h"

void AWizardHUD::CreateWizardOverlay()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && WizardOverlayClass)
	{
		WizardOverlay = CreateWidget<UWizardOverlay>(PlayerController, WizardOverlayClass);
		WizardOverlay->AddToViewport();
	}
}

void AWizardHUD::SetCurrentDistrict(EDistrict District)
{
	WizardOverlay->SetCurrentDistrictText(UEnum::GetDisplayValueAsText<EDistrict>(District));
}
