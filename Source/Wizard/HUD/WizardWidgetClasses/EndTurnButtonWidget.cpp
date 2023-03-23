// Fill out your copyright notice in the Description page of Project Settings.


#include "EndTurnButtonWidget.h"
#include "Sound/SoundCue.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Wizard/WizardTypes/InputContextTypes.h"
#include "Wizard/Controllers/WizardPlayerController.h"

void UEndTurnButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (EndTurnButton) EndTurnButton->OnClicked.AddDynamic(this, &UEndTurnButtonWidget::OnEndTurnButtonClicked);
}

void UEndTurnButtonWidget::SetIsButtonEnabled(bool bEnabled)
{ 
	if (EndTurnButton) EndTurnButton->SetIsEnabled(bEnabled); 
}

void UEndTurnButtonWidget::OnEndTurnButtonClicked()
{
	AWizardPlayerController* WController = Cast<AWizardPlayerController>(GetWorld()->GetFirstPlayerController());
	if (WController) {
		if (ClickSound) PlaySound(ClickSound);
		if (bPlayerEndedTurn) {
			FInputModeGameAndUI InputMode;
			WController->SetInputMode(InputMode);
			WController->SetInputContext(EInputContext::EIC_Default);
			WController->ServerCancelEndTurn();
			if (EndTurnText) EndTurnText->SetText(FText::FromString(TEXT("End Turn")));
			bPlayerEndedTurn = false;
		}
		else {
			FInputModeUIOnly InputMode;
			WController->SetInputMode(InputMode);
			WController->SetInputContext(EInputContext::EIC_None);
			WController->ServerEndTurn();
			if (EndTurnText) EndTurnText->SetText(FText::FromString(TEXT("Cancel")));
			bPlayerEndedTurn = true;
		}
	}
}
