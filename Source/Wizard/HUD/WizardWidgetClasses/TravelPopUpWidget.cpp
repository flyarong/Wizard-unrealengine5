// Fill out your copyright notice in the Description page of Project Settings.


#include "TravelPopUpWidget.h"
#include "Wizard/Characters/WizardCharacter.h"
#include "Wizard/Controllers/WizardPlayerController.h"
#include "Wizard/Components/Character/ActionComponent.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerController.h"

void UTravelPopUpWidget::PopUpSetup(EDistrict District)
{
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;
	DistrictToMove = District;
	if (NewDistrictNameText) NewDistrictNameText->SetText(UEnum::GetDisplayValueAsText<EDistrict>(DistrictToMove));

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeUIOnly InputModeData;
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
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeGameAndUI InputModeData;
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
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController) {
			AWizardCharacter* WizardCharacter = Cast<AWizardCharacter>(PlayerController->GetPawn());
			if (WizardCharacter && WizardCharacter->GetAction())
			{
				WizardCharacter->GetAction()->SetCurrentDistrict(DistrictToMove);
				PopUpTearDown();
			}
		}
	}

	if (YesButton) YesButton->SetIsEnabled(true);
}

void UTravelPopUpWidget::NoButtonClicked() {
	AWizardPlayerController* PlayerController = Cast<AWizardPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController) {
		PlayerController->CancelTravel();
		PopUpTearDown();
	}
}