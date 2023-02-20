// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputModifiers.h"
#include "InputModifierRotateMovement.generated.h"

/**
 * Input modifier for rotating the Camera on component Yaw
 */
UCLASS(meta = (DisplayName = "Rotate Camera Movement Yaw"))
class WIZARD_API UInputModifierRotateMovement : public UInputModifier
{
	GENERATED_BODY()
	
protected:
	
	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime) override;

	UPROPERTY()
	class AWizardPlayerController* PlayerController;
};
