// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "Wizard/WizardTypes/DistrictNames.h"
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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void AcknowledgePossession(APawn* Pawn) override;

	/// <summary>
	/// Function to initialize the Gameplay Overlay
	/// </summary>
	void InitOverlay();

	/// <summary>
	/// Function to stop the travelling to another
	/// district
	/// </summary>
	void CancelTravel();

#pragma region HUD
	/// <summary>
	/// Function to show the pop up message in the HUD
	/// before travelling to a new district
	/// </summary>
	void ShowHUDTravelPopUp(EDistrict District);

	/// <summary>
	/// Function for setting the current district
	/// in the HUD
	/// </summary>
	void SetHUDCurrentDistrict(EDistrict District, bool bMoveCharacter = false);

	/// <summary>
	/// Function to set the number of available Actions
	/// on the HUD
	/// </summary>
	void SetHUDActions(int32 Actions);

	/// <summary>
	/// Function to set the max number of Actions
	/// on the HUD
	/// </summary>
	void SetHUDNumOfActions(int32 NumOfActions);

	/// <summary>
	/// Set Point Of Interest on MiniMap
	/// in the HUD
	/// </summary>
	/// <param name="POIOwner">The owner of the Point Of Interest</param>
	void SetHUDPOIOnMiniMap(class AActor* POIOwner);
#pragma endregion

#pragma region InputPointers
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
#pragma endregion

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

#pragma region GameplayPointers
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
	/// Pointer to the HUD
	/// </summary>
	UPROPERTY()
	class AWizardHUD* WizardHUD;

	/// <summary>
	/// Pointer to the gameplay camera
	/// </summary>
	UPROPERTY()
	class AGameplayCamera* GameplayCamera;
#pragma endregion

	/// <summary>
	/// Boolean for whether or not the player's
	/// Overlay is initialized
	/// </summary>
	UPROPERTY()
	bool bWizardOverlayInitialized = false;

	/// <summary>
	/// Variable to store the character's current district
	/// until the HUD&Overlay becomes valid
	/// </summary>
	UPROPERTY()
	EDistrict CachedCurrentDistrict;

#pragma region Movement
	/// <summary>
	/// Server RPC to replicate Character movement
	/// </summary>
	/// <param name="Controller">Character's Controller</param>
	/// <param name="Dest">Location to move the Character to</param>
	UFUNCTION(Server, Reliable)
	void ServerMoveToLocation(AWizardPlayerController* Controller, FVector Dest);

	/// <summary>
	/// Server RPC to stop Character movement
	/// </summary>
	UFUNCTION(Server, Reliable)
	void ServerStopMovement();

	/// <summary>
	/// Vector storing the location where
	/// the character is moving from
	/// </summary>
	FVector CachedStart;

	/// <summary>
	/// Vector storing the location where
	/// the character is moving to
	/// </summary>
	UPROPERTY(Replicated)
	FVector CachedDestination;

	bool bIsTouch; // Is it a touch device
#pragma endregion
};


