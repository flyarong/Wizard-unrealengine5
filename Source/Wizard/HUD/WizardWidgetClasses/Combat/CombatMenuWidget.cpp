// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatMenuWidget.h"
#include "Components/Button.h"
#include "Wizard/Characters/WizardCharacter.h"
#include "Wizard/Components/Character/ActionComponent.h"
#include "Wizard/Controllers/WizardPlayerController.h"

void UCombatMenuWidget::BindEventsToButtons()
{
	StartButton->OnClicked.AddDynamic(this, &UCombatMenuWidget::OnStartButtonClicked);
	CancelButton->OnClicked.AddDynamic(this, &UCombatMenuWidget::OnCancelButtonClicked);
}

void UCombatMenuWidget::OnStartButtonClicked()
{
	AWizardCharacter* WCharacter = Cast<AWizardCharacter>(GetOwningPlayerPawn());
	if (WCharacter && WCharacter->GetAction()) {

	}
}

void UCombatMenuWidget::OnCancelButtonClicked()
{
	AWizardPlayerController* WController = Cast<AWizardPlayerController>(GetOwningPlayer());
	if (WController) {
		RemoveFromParent();
		if (StartButton->OnClicked.IsBound()) {
			StartButton->OnClicked.RemoveDynamic(this, &UCombatMenuWidget::OnStartButtonClicked);
		}
		if (CancelButton->OnClicked.IsBound()) {
			CancelButton->OnClicked.RemoveDynamic(this, &UCombatMenuWidget::OnCancelButtonClicked);
		}
		WController->SetCameraPositionToDefault();
	}
}
