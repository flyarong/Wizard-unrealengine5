// Fill out your copyright notice in the Description page of Project Settings.


#include "InputModifierMovementSpeed.h"
#include "EnhancedPlayerInput.h"
#include "Wizard/Controllers/WizardPlayerController.h"
#include "Wizard/Pawns/GameplayCamera.h"

FInputActionValue UInputModifierMovementSpeed::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime)
{
	FVector Value = CurrentValue.Get<FVector>();
	PlayerController = PlayerController == nullptr ?
		Cast<AWizardPlayerController>(PlayerInput->GetOuterAPlayerController()) : PlayerController;
	if (PlayerController && PlayerController->GetGameplayCamera()) {
		Value *= PlayerController->GetGameplayCamera()->GetCameraMovementSpeed();
	}

	return FInputActionValue(CurrentValue.GetValueType(), Value);
}
