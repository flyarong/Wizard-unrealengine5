// Fill out your copyright notice in the Description page of Project Settings.


#include "InputModifierRotateForward.h"

FInputActionValue UInputModifierRotateForward::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime)
{
	FVector Value = CurrentValue.Get<FVector>();
	FVector DeltaLocation = FRotator(0.f, -90.f, 0.f)
		.RotateVector(Value);

	return FInputActionValue(CurrentValue.GetValueType(), DeltaLocation);
}
