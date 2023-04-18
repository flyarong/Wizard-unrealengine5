// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "Wizard/Items/Item.h"
#include "Wizard/WizardTypes/DistrictNames.h"
#include "Wizard/WizardTypes/AttributeTypes.h"
#include "Wizard/WizardTypes/InputContextTypes.h"
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
	/// Client RPC to initialize the Gameplay Overlay
	/// </summary>
	UFUNCTION(Client, Reliable)
	void ClientInitOverlay();

	/// <summary>
	/// Function to setup the Gameplay Overlay once
	/// the Game has started
	/// </summary>
	void SetupOverlay();

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
	/// Function to set the current Input Mappings
	/// based on the type required by Gameplay situation
	/// </summary>
	/// <param name="ContextType">Type of the Input Context</param>
	void SetInputContext(EInputContext ContextType);

	/// <summary>
	/// Server RPC to trigger when the Player
	/// wants to end their turn
	/// </summary>
	UFUNCTION(Server, Reliable)
	void ServerEndTurn();

	/// <summary>
	/// Server RPC to trigger when the Player
	/// wants to cancel their turn ending
	/// </summary>
	UFUNCTION(Server, Reliable)
	void ServerCancelEndTurn();

	/// <summary>
	/// Function which is called when
	/// the MatchState changes in the GameMode
	/// </summary>
	/// <param name="NewMatchState">New MatchState</param>
	void OnMatchStateSet(FName NewMatchState);

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

	/// <summary>
	/// Function to set the number of Good/Dark Spells
	/// on the HUd
	/// </summary>
	/// <param name="NewSpell">Amount to set</param>
	/// <param name="IsGoodSpell">Whether the Spell is a GoodSpell or DarkSpell</param>
	void SetHUDSpells(int32 NewSpell, bool bIsGoodSpell = true);
#pragma endregion

#pragma region HUD/Items
	/// <summary>
	/// Function to set the store's catalog
	/// on the player's HUD
	/// </summary>
	/// <param name="Store">The store</param>
	void SetHUDStoreCatalog(class AStore* Store);

	/// <summary>
	/// Function to open the Inventory
	/// on the HUD
	/// </summary>
	UFUNCTION()
	void OpenHUDInventory();

	/// <summary>
	/// Function to update the Character's
	/// Inventory in the HUD
	/// </summary>
	void UpdateHUDCharacterInventory(const TArray<FItemDataTable>& Items);
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
	/// Client RPC to add a Public Message about
	/// a Character's victory in combat
	/// </summary>
	/// <param name="Character">The Character</param>
	/// <param name="DefeatedActor">Actor defeated</param>
	UFUNCTION(Client, Reliable)
	void ClientAddHUDVictoryPublicMessage(class AWizardCharacter* WCharacter, const TScriptInterface<class IWizardCombatActor>& DefeatedActor);

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
	/// Function to reset the HUD to default
	/// coming out from Combat
	/// </summary>
	void ResetHUD();

	/// <summary>
	/// Function to add the symbol of the current
	/// Spell Step to the HUD
	/// </summary>
	/// <param name="CurrentStepIndex">The current Step's index</param>
	void AddHUDCurrentSpellStep(int32 CurrentStepIndex);

	/// <summary>
	/// Function to remove the previous Spell Step from the HUD
	/// </summary>
	void RemoveHUDPreviouseSpellStep();

	/// <summary>
	/// Function to add the Result of the current Step
	/// to the HUD
	/// </summary>
	/// <param name="bWasSuccessful">Whether it was successful or not</param>
	void AddHUDSpellStepResult(bool bWasSuccessful);

	/// <summary>
	/// Function to add the current Combat Score
	/// on the HUD
	/// </summary>
	/// <param name="Score">The Combat Score</param>
	void AddHUDCombatScore(int32 Score);
#pragma endregion

#pragma region HUD/MatchStates
	/// <summary>
	/// Function that sets up the HUD
	/// after the Turn has ended
	/// </summary>
	void SetupHUDPostTurn();

	/// <summary>
	/// Function that sets up the HUD
	/// before the new Turn starts
	/// </summary>
	void SetupHUDPreTurn();

	/// <summary>
	/// Function to set the HUD based
	/// on the MatchState
	/// </summary>
	void SetHUDMatchState();

	/// <summary>
	/// Function to add the current MatchState
	/// to the HUD
	/// </summary>
	void AddHUDMatchState();

	/// <summary>
	/// Client RPC to update the number of
	/// Story Points on the HUD
	/// </summary>
	/// <param name="NumOfPoints">Updated number of points</param>
	/// <param name="bIsPositiveStoryPoints">Whether or not the updated Story Points refer to Positive Story points or not</param>
	UFUNCTION(Client, Reliable)
	void ClientSetHUDStoryPoints(const float& NumOfPoints, bool bIsPositiveStoryPoints);
#pragma endregion

#pragma region InputPointers
	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

	/** Enhanced Input Subsystem */
	UPROPERTY()
	class UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem;

	/** Default MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;
	
	/** Click Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* SetDestinationClickAction;

	/** Camera Movement Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* CameraMoveAction;

	/** Camera Zoom Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* CameraZoomAction;

	/** Camera Rotation Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* CameraRotateAction;

	/** Combat MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* CombatMappingContext;

	/** Combat Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* CombatAction;
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

	/// <summary>
	/// Callback function to Mouse Wheel Axis for
	/// Zooming in/out with the camera
	/// </summary>
	void OnMouseWheelAxis(const FInputActionValue& ActionValue);

	/// <summary>
	/// Callback function to rotate Camera when Right Mouse Button
	/// is held down
	/// </summary>
	void OnMouseRotateYaw(const FInputActionValue& ActionValue);

	/// <summary>
	/// Callback function for moving the Camera
	/// with keys
	/// </summary>
	void OnKeyMove(const FInputActionValue& ActionValue);

	/// <summary>
	/// Callback function for Combat started input event
	/// </summary>
	void OnCombatKeyStarted(const FInputActionValue& ActionValue);

	/// <summary>
	/// Boolean for whether or not the Camera can
	/// be moved with Input
	/// </summary>
	UPROPERTY()
	bool bCanCameraMove = true;
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
	/// Pointer to the GameState
	/// </summary>
	UPROPERTY()
	class AWizardGameState* WizardGameState;

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
	/// Variable storing the current MatchState
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_MatchState)
	FName MatchState;

	UFUNCTION()
	void OnRep_MatchState();

	/// <summary>
	/// Boolean for whether or not the Player
	/// is in Combat
	/// </summary>
	UPROPERTY(Replicated)
	bool bCanCastSpell = false;

#pragma region Movement
	/// <summary>
	/// Server RPC to replicate Character movement
	/// </summary>
	/// <param name="Controller">Character's Controller</param>
	/// <param name="Dest">Location to move the Character to</param>
	UFUNCTION(Server, Reliable)
	void ServerMoveToLocation(AWizardPlayerController* Controller, const FVector& Dest);

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
#pragma endregion

public:
	FORCEINLINE AWizardCharacter* GetWizardCharacter() const { return WizardCharacter; }
	FORCEINLINE AGameplayCamera* GetGameplayCamera() const { return GameplayCamera; }
	FORCEINLINE bool GetCanCastSpell() const { return bCanCastSpell; }
	FORCEINLINE void SetCanCastSpell(bool bIsInCombat) { bCanCastSpell = bIsInCombat; }
	FORCEINLINE FVector GetCachedDestination() const { return CachedDestination; }
};


