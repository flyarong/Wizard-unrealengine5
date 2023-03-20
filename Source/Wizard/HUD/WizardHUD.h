// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Wizard/WizardTypes/DistrictNames.h"
#include "Wizard/WizardTypes/AttributeTypes.h"
#include "Wizard/WizardTypes/ActionTypes.h"
#include "Wizard/Items/Item.h"
#include "WizardHUD.generated.h"

/**
 * HUD class for the gameplay
 */
UCLASS()
class WIZARD_API AWizardHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	/// <summary>
	/// Create & add the overlay to the viewport
	/// </summary>
	/// <returns>Whether the creation was successful or not</returns>
	bool CreateWizardOverlay();

#pragma region General
	/// <summary>
	/// Function to clear the Top Right Box
	/// in the Overlay
	/// </summary>
	void ClearTopRightBox();

	/// <summary>
	/// Function to clear the Top Right Box
	/// in the Overlay
	/// </summary>
	void ClearCenterBox();

	/// <summary>
	/// Function to clear the Bottom Box in the Overlay
	/// </summary>
	void ClearBottomBox();

	/// <summary>
	/// Function to hide the Top Center Box from the Overlay
	/// </summary>
	void HideTopCenterBox();

	/// <summary>
	/// Function to show the Left Side Panel
	/// on the Overlay
	/// </summary>
	void ShowLeftPanel();

	/// <summary>
	/// Function to hide the Left Side Panel
	/// from the Overlay
	/// </summary>
	void HideLeftPanel();

	/// <summary>
	/// Function to show the Right Side Panel
	/// on the Overlay
	/// </summary>
	void ShowRightPanel();

	/// <summary>
	/// Function to hide the Right Side Panel
	/// from the Overlay
	/// </summary>
	void HideRightPanel();
#pragma endregion

#pragma region Player
	/// <summary>
	/// Function for printing the current district's name
	/// on the screen
	/// </summary>
	/// <param name="District">The current district to print</param>
	void SetCurrentDistrict(EDistrict District);

	/// <summary>
	/// Function to set the Health level
	/// on the HUD
	/// </summary>
	void SetHealth(float Health, float MaxHealth);

	/// <summary>
	/// Function to set the Power level
	/// on the HUD
	/// </summary>
	void SetPower(float Power, float MaxPower);

	/// <summary>
	/// Function to set the Character's image
	/// on the HUD
	/// </summary>
	void SetCharacterImage(UTexture2D* CharacterImage);

	/// <summary>
	/// Function to set the Character's name
	/// on the HUD
	/// </summary>
	void SetCharacterName(FString CharacterName);

	/// <summary>
	/// Function to set the new XP value
	/// on the HUD
	/// </summary>
	void SetXP(int32 NewXP);

	/// <summary>
	/// Function to set the new Offense value
	/// on the HUD
	/// </summary>
	void SetOffense(int32 NewOffense);

	/// <summary>
	/// Function to set the new Defense value
	/// on the HUD
	/// </summary>
	void SetDefense(int32 NewDefense);

	/// <summary>
	/// Function to set the new Wisdom value
	/// on the HUD
	/// </summary>
	void SetWisdom(int32 NewWisdom);

	/// <summary>
	/// Function to set the new Intelligence value
	/// on the HUD
	/// </summary>
	void SetIntelligence(int32 NewIntelligence);

	/// <summary>
	/// Function to set the new Agility value
	/// on the HUD
	/// </summary>
	void SetAgility(int32 NewAgility);

	/// <summary>
	/// Function to set the new Spell value
	/// on the HUD
	/// </summary>
	/// <param name="NewSpell">The new Spell amount</param>
	/// <param name="bIsGoodSpell">Whether the Spell is a Good Spell or a Dark Spell</param>
	void SetSpells(int32 NewSpell, bool bIsGoodSpell);
#pragma endregion

#pragma region Items
	/// <summary>
	/// Function to show the currently browsed store's
	/// catalog on the HUD
	/// </summary>
	/// <param name="Store">The store</param>
	void SetStoreCatalog(class AStore* Store);

	/// <summary>
	/// Function to update the Character's Inventory
	/// </summary>
	void UpdateCharacterInventory(const TArray<FItemDataTable>& Items);

	/// <summary>
	/// Function to show the Character's Inventory 
	/// on the Overlay
	/// </summary>
	void ShowCharacterInventory();
#pragma endregion

#pragma region MiniMap
	/// <summary>
	/// Function to set the Point Of Interest on the MiniMap 
	/// on the HUD
	/// </summary>
	/// <param name="POIOwner">Point Of Interest's Owner</param>
	void SetPOIOnMiniMap(class AActor* POIOwner);
#pragma endregion

#pragma region Messages
	/// <summary>
	/// Function to add a Local Message to the screen
	/// </summary>
	/// <param name="Message">The Message</param>
	/// <param name="AttributeType">The Attribute the message refers to</param>
	void AddLocalMessage(const FString& Message, EAttribute AttributeType);

	/// <summary>
	/// Function to add a Public Message to the Overlay
	/// </summary>
	/// <param name="Source">Source of the Event</param>
	/// <param name="EventAction">Event type</param>
	/// <param name="Target">Event Target</param>
	void AddPublicMessage(class IPublicMessageActor* Source, EAction EventAction, IPublicMessageActor* Target);

	/// <summary>
	/// Function to add a Chat Message to the ChatBox
	/// </summary>
	/// <param name="Message">The message written in chat</param>
	void AddChatMessage(const FText& Message);
#pragma endregion

#pragma region Combat
	/// <summary>
	/// Function to create the SpellMap
	/// Widget
	/// </summary>
	/// <param name="SpellMap">The SpellMap</param>
	void CreateSpellMap(TMap<FKey, int32>& SpellMap);
	
	/// <summary>
	/// Function to add the SpellMap to the Overlay
	/// </summary>
	void AddSpellMap();

	/// <summary>
	/// Function to remove the SpellMap from the Overlay
	/// </summary>
	void RemoveSpellMap();

	/// <summary>
	/// Function to add the Combat Menu to the Overlay
	/// </summary>
	void AddCombatMenu();

	/// <summary>
	/// Function to add the current Spell Step
	/// to the Overlay
	/// </summary>
	/// <param name="CurrentSpellStep">Index of the current Spell Step</param>
	void AddCurrentSpellStep(int32 CurrentSpellStep);

	/// <summary>
	/// Function to add the current Step result
	/// to the Overlay
	/// </summary>
	/// <param name="bWasSuccessful">Whether the result was successful or not</param>
	void AddSpellStepResult(bool bWasSuccessful);

	/// <summary>
	/// Function to create the Combat Score
	/// on the Overlay
	/// </summary>
	/// <param name="Score">The Combat Score</param>
	void CreateCombatScore();

	/// <summary>
	/// Function to add the Combat Score
	/// to the Overlay
	/// </summary>
	/// <param name="Score">The Combat Score</param>
	void AddCombatScore(int32 Score);
#pragma endregion

private:

	/// <summary>
	/// Widget Class of the gameplay overlay
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Wizard Overlay")
	TSubclassOf<class UUserWidget> WizardOverlayClass;

	/// <summary>
	/// Pointer to the gameplay overlay
	/// </summary>
	UPROPERTY()
	class UWizardOverlay* WizardOverlay;

public:
	FORCEINLINE UWizardOverlay* GetOverlay() const { return WizardOverlay; }
};
