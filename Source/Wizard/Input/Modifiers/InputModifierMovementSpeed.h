// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputModifiers.h"
#include "InputModifierMovementSpeed.generated.h"

/**
 * Input modifier for setting the Camera's movement speed
 */
UCLASS(meta = (DisplayName = "Set Camera Movement Speed"))
class WIZARD_API UInputModifierMovementSpeed : public UInputModifier
{
	GENERATED_BODY()
	
protected:

	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime) override;

	UPROPERTY()
	class AWizardPlayerController* PlayerController;
};
