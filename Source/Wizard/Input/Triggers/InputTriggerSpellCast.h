// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputTriggers.h"
#include "InputTriggerSpellCast.generated.h"

/**
 * Input trigger for whether or not Combat is
 * enabled for Casting Spells
 */
UCLASS(meta = (DisplayName = "In Combat"))
class WIZARD_API UInputTriggerSpellCast : public UInputTrigger
{
	GENERATED_BODY()
	
protected:

	virtual ETriggerState UpdateState_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue ModifiedValue, float DeltaTime) override;

	UPROPERTY()
	class AWizardPlayerController* PlayerController;
};
