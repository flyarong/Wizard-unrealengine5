// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputModifiers.h"
#include "InputModifierCombatKey.generated.h"

/**
 * Input modifier for the Left Arrow Combat Key
 */
UCLASS(meta = (DisplayName = "Left Combat Key Modifier"))
class WIZARD_API UInputModifierLeftArrowKey : public UInputModifier
{
	GENERATED_BODY()
	
protected:
	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime) override;
};

/**
 * Input modifier for the Right Arrow Combat Key
 */
UCLASS(meta = (DisplayName = "Right Combat Key Modifier"))
class WIZARD_API UInputModifierRightArrowKey : public UInputModifier
{
	GENERATED_BODY()

protected:
	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime) override;
};

/**
 * Input modifier for the Up Arrow Combat Key
 */
UCLASS(meta = (DisplayName = "Up Combat Key Modifier"))
class WIZARD_API UInputModifierUpArrowKey : public UInputModifier
{
	GENERATED_BODY()

protected:
	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime) override;
};

/**
 * Input modifier for the Down Arrow Combat Key
 */
UCLASS(meta = (DisplayName = "Down Combat Key Modifier"))
class WIZARD_API UInputModifierDownArrowKey : public UInputModifier
{
	GENERATED_BODY()

protected:
	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime) override;
};
