// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputModifiers.h"
#include "InputModifierRotateForward.generated.h"

/**
 * Input modifier for rotating the Camera to forward axis
 */
UCLASS(meta = (DisplayName = "Rotate Camera Forward Movement Yaw"))
class WIZARD_API UInputModifierRotateForward : public UInputModifier
{
	GENERATED_BODY()

protected:

	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime) override;

	UPROPERTY()
	class AWizardPlayerController* PlayerController;
};
