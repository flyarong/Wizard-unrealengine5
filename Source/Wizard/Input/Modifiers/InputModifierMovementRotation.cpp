// Fill out your copyright notice in the Description page of Project Settings.


#include "InputModifierMovementRotation.h"
#include "EnhancedPlayerInput.h"
#include "Wizard/Controllers/WizardPlayerController.h"
#include "Wizard/Pawns/GameplayCamera.h"

FInputActionValue UInputModifierRotateMovement::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime)
{
	FVector Value = CurrentValue.Get<FVector>();
	FVector DeltaLocation = FVector::ZeroVector;
	float MovementSpeed = 1.f;
	PlayerController = PlayerController == nullptr ?
		Cast<AWizardPlayerController>(PlayerInput->GetOuterAPlayerController()) : PlayerController;
	if (PlayerController && PlayerController->GetGameplayCamera()) {
		DeltaLocation = FRotator(0.f, PlayerController->GetGameplayCamera()->GetActorRotation().Yaw, 0.f)
			.RotateVector(FVector(0.f, Value.Y, 0.f));
		MovementSpeed = PlayerController->GetGameplayCamera()->GetCameraMovementSpeed();
	}

	return FInputActionValue(CurrentValue.GetValueType(), DeltaLocation);
}

FInputActionValue UInputModifierRotateForward::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime)
{
	FVector Value = CurrentValue.Get<FVector>();
	FVector DeltaLocation = FRotator(0.f, -90.f, 0.f)
		.RotateVector(Value);

	return FInputActionValue(CurrentValue.GetValueType(), DeltaLocation);
}