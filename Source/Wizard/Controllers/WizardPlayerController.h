// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "Wizard/WizardTypes/DistrictNames.h"
#include "Wizard/WizardTypes/AttributeTypes.h"
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

	/// <summary>
	/// Function to setup the Gameplay Camera at the
	/// beginning of the Game
	/// </summary>
	void SetupCamera();

	/// <summary>
	/// Function to initialize the Gameplay Overlay
	/// </summary>
	void InitOverlay();

	/// <summary>
	/// Function to set the focus of the Camera
	/// to the Character
	/// </summary>
	UFUNCTION()
	void SetCameraFocusOnWizard();

	/// <summary>
	/// Function to set the Camera's position
	/// to the Default
	/// </summary>
	void SetCameraPositionToDefault();

	/// <summary>
	/// Function to set the Camera's position
	/// for Combat
	/// </summary>
	void SetCameraPositionToCombat();

	/// <summary>
	/// Function to enable/disable Character&Camera movement
	/// </summary>
	/// <param name="bIsMovementEnabled">Whether movement should be enabled or not</param>
	void SetWizardMovementIsEnabled(bool bIsMovementEnabled);

#pragma region HUD/Player
	/// <summary>
	/// Function for setting the current district
	/// in the HUD
	/// </summary>
	void SetHUDCurrentDistrict(EDistrict District);

	/// <summary>
	/// Function to set the Health bar
	/// on the HUD
	/// </summary>
	void SetHUDHealth(float Health, float MaxHealth);

	/// <summary>
	/// Function to set the Power bar
	/// on the HUD
	/// </summary>
	void SetHUDPower(float Power, float MaxPower);

	/// <summary>
	/// Function to set the Character's image
	/// on the HUD
	/// </summary>
	void SetHUDCharacterImage(UTexture2D* CharacterImage);

	/// <summary>
	/// Function to set the Character's name
	/// on the HUD
	/// </summary>
	void SetHUDCharacterName(FString CharacterName);

	/// <summary>
	/// Function to set the new value of
	/// XP on the HUD
	/// </summary>
	void SetHUDXP(int32 NewXP);

	/// <summary>
	/// Function to set the new Offense value
	/// on the HUD
	/// </summary>
	void SetHUDOffense(int32 NewOffense);

	/// <summary>
	/// Function to set the new Defense value
	/// on the HUD
	/// </summary>
	void SetHUDDefense(int32 NewDefense);

	/// <summary>
	/// Function to set the new Wisdom value
	/// on the HUD
	/// </summary>
	void SetHUDWisdom(int32 NewWisdom);

	/// <summary>
	/// Function to set the new Intelligence value
	/// on the HUD
	/// </summary>
	void SetHUDIntelligence(int32 NewIntelligence);

	/// <summary>
	/// Function to set the new Agility value
	/// on the HUD
	/// </summary>
	void SetHUDAgility(int32 NewAgility);
#pragma endregion

#pragma region HUD/Store/Catalog
	/// <summary>
	/// Function to set the store's catalog
	/// on the player's HUD
	/// </summary>
	/// <param name="Store">The store</param>
	void SetHUDStoreCatalog(class AStore* Store);

	/// <summary>
	/// Function to add a new Item
	/// to the Character's Items list on the HUD
	/// </summary>
	void AddHUDCharacterItem(int32 ItemIndex);
#pragma endregion

#pragma region HUD/MiniMap
	/// <summary>
	/// Set Point Of Interest on MiniMap
	/// in the HUD
	/// </summary>
	/// <param name="POIOwner">The owner of the Point Of Interest</param>
	void SetHUDPOIOnMiniMap(class AActor* POIOwner);

	/// <summary>
	/// Server RPC to destroy the POI from
	/// GameMode
	/// </summary>
	/// <param name="POIOwner">The Actor to remove from the MiniMap</param>
	UFUNCTION(Server, Reliable)
	void ServerDestroyPOI(AActor* POIOwner);
#pragma endregion

#pragma region HUD/Messages
	/// <summary>
	/// Function to add a Local Message to the HUD
	/// </summary>
	/// <param name="Message">The Message</param>
	/// <param name="AttributeType">The Attribute the message refers to</param>
	void AddHUDLocalMessage(const FString& Message, EAttribute AttributeType);

	/// <summary>
	/// Server RPC for sending Chat messages
	/// </summary>
	/// <param name="Sender">Name of the player who sends the message</param>
	/// <param name="Message">Message to send</param>
	UFUNCTION(Server, Reliable)
	void ServerSendChatMessage(const FString& Sender, const FText& Message);

	/// <summary>
	/// Function to add a Chat Message to the HUD
	/// </summary>
	/// <param name="Message">The message written in chat</param>
	UFUNCTION(Client, Reliable)
	void ClientAddHUDChatMessage(const FText& Message);
#pragma endregion

#pragma region HUD/Combat
	
	/// <summary>
	/// Function to create the SpellMap
	/// in the HUD
	/// </summary>
	/// <param name="SpellInputs">The array of inputs</param>
	/// <param name="SpellIndexes">The array of texture indexes</param>
	void CreateHUDSpellMap(TArray<FKey>& SpellInputs, TArray<int32>& SpellIndexes);

	/// <summary>
	/// Function to add the SpellMap
	/// to the HUD
	/// </summary>
	void AddHUDSpellMap();

	/// <summary>
	/// Function to remove the SpellMap from the HUD
	/// </summary>
	void RemoveSpellMapFromHUD();

	/// <summary>
	/// Function to add the Combat Menu
	/// to the HUD
	/// </summary>
	void AddHUDCombatMenu();

	/// <summary>
	/// Function to add the symbol of the current
	/// Spell Step to the HUD
	/// </summary>
	/// <param name="CurrentStepIndex">The current Step's index</param>
	void AddHUDCurrentSpellStep(int32 CurrentStepIndex);
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
	void OnMouseWheelAxis(float Value);

	/// <summary>
	/// Callback function to rotate Camera with Right Mouse Button
	/// </summary>
	void OnMouseRotateYaw(float Value);

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
	/// the character is moving to
	/// </summary>
	UPROPERTY(Replicated)
	FVector CachedDestination;

	/// <summary>
	/// Boolean for whether or not the Character
	/// can move
	/// </summary>
	UPROPERTY()
	bool bCanCharacterMove = true;

	/// <summary>
	/// Boolean for whether or not the Camera
	/// can move
	/// </summary>
	UPROPERTY()
	bool bCanCameraMove = true;

	bool bIsTouch; // Is it a touch device
#pragma endregion

public:
	void SetWizardCharacter(AWizardCharacter* WCharacter);
	FORCEINLINE AWizardCharacter* GetWizardCharacter() const { return WizardCharacter; }
};


