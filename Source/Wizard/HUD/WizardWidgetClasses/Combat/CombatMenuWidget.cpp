// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatMenuWidget.h"
#include "Components/Button.h"
#include "Wizard/Controllers/WizardPlayerController.h"
#include "Wizard/Characters/WizardCharacter.h"
#include "Wizard/Components/Character/ActionComponent.h"
#include "Wizard/Components/Character/AttributeComponent.h"
#include "Wizard/Components/Character/CombatComponent.h"

void UCombatMenuWidget::BindEventsToButtons()
{
	AWizardCharacter* WCharacter = Cast<AWizardCharacter>(GetOwningPlayerPawn());
	if (WCharacter && WCharacter->GetCombat() && WCharacter->GetAttribute()) {
		StartButton->OnClicked.AddDynamic(this, &UCombatMenuWidget::OnStartButtonClicked);
		if (WCharacter->GetCombat()->GetIsAttacking() && WCharacter->GetCombat()->GetCombatTarget() &&
			!WCharacter->GetCombat()->GetCombatTarget()->ActorHasTag(FName("Trial"))) {
			CancelButton->OnClicked.AddDynamic(this, &UCombatMenuWidget::OnCancelButtonClicked);
		}
		else { // Can't cancel defending & trial
			CancelButton->RemoveFromParent();
		}

		if (WCharacter->GetCombat()->GetCombatTarget() && WCharacter->GetCombat()->GetCombatTarget()->ActorHasTag(FName("Enemy")) &&
			WCharacter->GetAttribute()->GetGoodSpells() > 0) {
			GoodSpellButton->OnClicked.AddDynamic(this, &UCombatMenuWidget::OnGoodSpellButtonClicked);
		}
		else {
			GoodSpellButton->RemoveFromParent();
		}

		if (WCharacter->GetCombat()->GetCombatTarget() && WCharacter->GetCombat()->GetCombatTarget()->ActorHasTag(FName("Enemy")) &&
			WCharacter->GetAttribute()->GetDarkSpells() > 0) {
			DarkSpellButton->OnClicked.AddDynamic(this, &UCombatMenuWidget::OnDarkSpellButtonClicked);
		}
		else {
			DarkSpellButton->RemoveFromParent();
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

void UCombatMenuWidget::OnDarkSpellButtonClicked()
{
	AWizardCharacter* WCharacter = Cast<AWizardCharacter>(GetOwningPlayerPawn());
	if (WCharacter && WCharacter->GetAction()) {
		AWizardPlayerController* WController = WCharacter->GetWizardController();
		if (WController) WController->SetShowMouseCursor(false);
		RemoveFromParent();
		WCharacter->GetAction()->ServerStartDarkSpellCombat();
	}
}

void UCombatMenuWidget::OnGoodSpellButtonClicked()
{
	AWizardCharacter* WCharacter = Cast<AWizardCharacter>(GetOwningPlayerPawn());
	if (WCharacter && WCharacter->GetAction()) {
		AWizardPlayerController* WController = WCharacter->GetWizardController();
		if (WController) WController->SetShowMouseCursor(false);
		RemoveFromParent();
		WCharacter->GetAction()->ServerStartGoodSpellCombat();
	}
}
