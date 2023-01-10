// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "WizardPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;

UCLASS()
class AWizardPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AWizardPlayerController();
	virtual void Tick(float DeltaSeconds) override;

	/// <summary>
	/// Function to initialize the WizardCharacter
	/// </summary>
	/// <param name="CharacterName">Selected Character's name</param>
	void InitCharacter(FName CharacterName);

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;
	
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* SetDestinationClickAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* SetDestinationTouchAction;

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	virtual void SetupInputComponent() override;
	
	// To add mapping context
	virtual void BeginPlay();

#pragma region InputHandling
	/** Input handlers for SetDestination action. */
	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();
	void OnTouchTriggered();
	void OnTouchReleased();

	/// <summary>
	/// Callback function to Mouse Wheel Axis for
	/// Zooming in/out with the camera
	/// </summary>
	void MouseWheelAxis(float Value);

	/// <summary>
	/// Callback function to rotate Camera with Right Mouse Button
	/// </summary>
	void MouseRotateYaw(float Value);

	/// <summary>
	/// Callback function for moving the camera
	/// forward/backward with keys
	/// </summary>
	void OnKeyMoveForward(float Value);

	/// <summary>
	/// Callback function for moving the camera
	/// left/right with keys
	/// </summary>
	void OnKeyMoveRight(float Value);
#pragma endregion

private:

	/// <summary>
	/// Pointer to the player character
	/// </summary>
	UPROPERTY()
	class AWizardCharacter* WizardCharacter;

	/// <summary>
	/// Pointer to the GameMode
	/// </summary>
	UPROPERTY()
	class AWizardGameMode* WizardGameMode;

	/// <summary>
	/// Pointer to the PlayerState
	/// </summary>
	UPROPERTY()
	class AWizardPlayerState* WizardPlayerState;

	/// <summary>
	/// Pointer to the gameplay camera
	/// </summary>
	UPROPERTY()
	class AGameplayCamera* GameplayCamera;

	/// <summary>
	/// Boolean for whether or not the player's
	/// WizardCharacter is initialized
	/// </summary>
	UPROPERTY()
	bool bCharacterInitialized = false;

#pragma region Movement
	/// <summary>
	/// Server RPC to replicate Character movement
	/// </summary>
	/// <param name="Controller">Character's Controller</param>
	/// <param name="Dest">Location to move the Character to</param>
	UFUNCTION(Server, Reliable)
	void ServerMoveToLocation(AWizardPlayerController* Controller, FVector Dest);

	FVector CachedDestination;

	bool bIsTouch; // Is it a touch device
#pragma endregion
};


