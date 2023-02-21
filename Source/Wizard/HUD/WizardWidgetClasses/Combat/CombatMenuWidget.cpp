// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatMenuWidget.h"
#include "Components/Button.h"
#include "Wizard/Controllers/WizardPlayerController.h"
#include "Wizard/Characters/WizardCharacter.h"
#include "Wizard/Components/Character/ActionComponent.h"

void UCombatMenuWidget::BindEventsToButtons()
{
	StartButton->OnClicked.AddDynamic(this, &UCombatMenuWidget::OnStartButtonClicked);
	CancelButton->OnClicked.AddDynamic(this, &UCombatMenuWidget::OnCancelButtonClicked);
}

void UCombatMenuWidget::OnStartButtonClicked()
{
	AWizardCharacter* WCharacter = Cast<AWizardCharacter>(GetOwningPlayerPawn());
	if (WCharacter && WCharacter->GetAction()) {
		AWizardPlayerController* WController = WCharacter->GetWizardController();
		if (WController) WController->SetShowMouseCursor(false);
		RemoveFromParent();
		WCharacter->GetAction()->ServerStartCombat();
	}
}

void UCombatMenuWidget::OnCancelButtonClicked()
{
	AWizardCharacter* WCharacter = Cast<AWizardCharacter>(GetOwningPlayerPawn());
	if (WCharacter && WCharacter->GetAction()) {
		RemoveFromParent();
		WCharacter->GetAction()->ServerCancelCombat();
	}
}
