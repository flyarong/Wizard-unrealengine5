// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputTriggers.h"
#include "InputTriggerRightMouse.generated.h"

/**
 * Input trigger for whether or not the Right Mouse
 * Button is down or not
 */
UCLASS(meta = (DisplayName = "Right Mouse Down"))
class WIZARD_API UInputTriggerRightMouse : public UInputTrigger
{
	GENERATED_BODY()
	
protected:

	virtual ETriggerState UpdateState_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue ModifiedValue, float DeltaTime) override;

	UPROPERTY()
	class APlayerController* PlayerController;
};
