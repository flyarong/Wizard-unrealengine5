// Fill out your copyright notice in the Description page of Project Settings.


#include "InputModifierCombatKey.h"

FInputActionValue UInputModifierLeftArrowKey::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime)
{
	return CurrentValue.Get<float>() * 1.f;
}

FInputActionValue UInputModifierRightArrowKey::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime)
{
	return CurrentValue.Get<float>() * 3.f;
}

FInputActionValue UInputModifierUpArrowKey::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime)
{
	return CurrentValue.Get<float>() * 0.f;
}

FInputActionValue UInputModifierDownArrowKey::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime)
{
	return CurrentValue.Get<float>() * 2.f;
}
