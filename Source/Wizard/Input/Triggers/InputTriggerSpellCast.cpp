// Fill out your copyright notice in the Description page of Project Settings.


#include "InputTriggerSpellCast.h"
#include "EnhancedPlayerInput.h"
#include "Wizard/Controllers/WizardPlayerController.h"

ETriggerState UInputTriggerSpellCast::UpdateState_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue ModifiedValue, float DeltaTime)
{
	PlayerController = PlayerController == nullptr ?
		Cast<AWizardPlayerController>(PlayerInput->GetOuterAPlayerController()) : PlayerController;
	if (PlayerController && PlayerController->GetCanCastSpell()) return ETriggerState::Triggered;

	return ETriggerState::None;
}
