// Fill out your copyright notice in the Description page of Project Settings.


#include "InputTriggerRightMouse.h"
#include "EnhancedPlayerInput.h"
#include "GameFramework/PlayerController.h"

ETriggerState UInputTriggerRightMouse::UpdateState_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue ModifiedValue, float DeltaTime)
{
	PlayerController = PlayerController == nullptr ? PlayerInput->GetOuterAPlayerController() : PlayerController;
	if (PlayerController && PlayerController->IsInputKeyDown(EKeys::RightMouseButton)) {
		return ETriggerState::Triggered;
	}

	return ETriggerState::None;
}
