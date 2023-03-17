// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatMenuWidget.h"
#include "Components/Button.h"
#include "Wizard/Controllers/WizardPlayerController.h"
#include "Wizard/Characters/WizardCharacter.h"
#include "Wizard/Components/Character/ActionComponent.h"
#include "Wizard/Components/Character/CombatComponent.h"

void UCombatMenuWidget::BindEventsToButtons()
{
	AWizardCharacter* WCharacter = Cast<AWizardCharacter>(GetOwningPlayerPawn());
	if (WCharacter && WCharacter->GetCombat()) {
		StartButton->OnClicked.AddDynamic(this, &UCombatMenuWidget::OnStartButtonClicked);
		if (WCharacter->GetCombat()->GetIsAttacking()) {
			CancelButton->OnClicked.AddDynamic(this, &UCombatMenuWidget::OnCancelButtonClicked);
		}
		else { // Can't cancel defending
			CancelButton->RemoveFromParent();
		}
	}
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
