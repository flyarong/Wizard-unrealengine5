// Fill out your copyright notice in the Description page of Project Settings.


#include "Wizard/Characters/WizardCharacter.h"
#include "Wizard/Components/Character/ActionComponent.h"
#include "TravelPopUpWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Wizard/Controllers/WizardPlayerController.h"

void UTravelPopUpWidget::PopUpSetup(EDistrict District)
{
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;
	DistrictToMove = District;
	if (NewDistrictNameText) NewDistrictNameText->SetText(UEnum::GetDisplayValueAsText<EDistrict>(District));

	UWorld* World = GetWorld();
	if (World)
	{
		PlayerController = PlayerController == nullptr ? Cast<AWizardPlayerController>(World->GetFirstPlayerController()) : PlayerController;
		if (PlayerController)
		{
			FInputModeGameAndUI InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}
	}
	if (YesButton && !YesButton->OnClicked.IsBound())
	{
		YesButton->OnClicked.AddDynamic(this, &UTravelPopUpWidget::YesButtonClicked);
	}
	if (NoButton && !NoButton->OnClicked.IsBound())
	{
		NoButton->OnClicked.AddDynamic(this, &UTravelPopUpWidget::NoButtonClicked);
	}
}

void UTravelPopUpWidget::PopUpTearDown() {
	RemoveFromParent();
	UWorld* World = GetWorld();
	if (World)
	{
		PlayerController = PlayerController == nullptr ? Cast<AWizardPlayerController>(World->GetFirstPlayerController()) : PlayerController;
		if (PlayerController)
		{
			FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}
	}
	if (YesButton && YesButton->OnClicked.IsBound())
	{
		YesButton->OnClicked.RemoveDynamic(this, &UTravelPopUpWidget::YesButtonClicked);
	}
	if (NoButton && NoButton->OnClicked.IsBound())
	{
		NoButton->OnClicked.RemoveDynamic(this, &UTravelPopUpWidget::NoButtonClicked);
	}
}

void UTravelPopUpWidget::YesButtonClicked() {
	YesButton->SetIsEnabled(false);

	UWorld* World = GetWorld();
	if (World)
	{
		PlayerController = PlayerController == nullptr ? Cast<AWizardPlayerController>(World->GetFirstPlayerController()) : PlayerController;
		AWizardCharacter* WizardCharacter = Cast<AWizardCharacter>(PlayerController->GetPawn());
		if (PlayerController && WizardCharacter && WizardCharacter->GetAction())
		{
			// TODO decrease number of actions in PlayerState
			WizardCharacter->GetAction()->SetCurrentDistrict(DistrictToMove);
			PlayerController->SetHUDCurrentDistrict(DistrictToMove, true);
		}
	}

	if (YesButton) YesButton->SetIsEnabled(true);
}

void UTravelPopUpWidget::NoButtonClicked() {
	PopUpTearDown();
}