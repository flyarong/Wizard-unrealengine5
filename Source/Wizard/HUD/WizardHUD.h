// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Wizard/WizardTypes/DistrictNames.h"
#include "Wizard/WizardTypes/AttributeTypes.h"
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
#pragma endregion

#pragma region Store/Catalog
	/// <summary>
	/// Function to show the currently browsed store's
	/// catalog on the HUD
	/// </summary>
	/// <param name="Store">The store</param>
	void SetStoreCatalog(class AStore* Store);

	/// <summary>
	/// Function to add a new item to the
	/// Character's Item Panel
	/// </summary>
	void AddCharacterItem(int32 ItemIndex);
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
	/// Function to add a Chat Message to the ChatBox
	/// </summary>
	/// <param name="Message">The message written in chat</param>
	void AddChatMessage(const FText& Message);
#pragma endregion

	void AddSpellMap(TMap<FKey, class UTexture2D*>& SpellMap);

	void AddCombatMenu();

private:
	
	APlayerController* PlayerController;

	/// <summary>
	/// Widget Class of the gameplay overlay
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Wizard Overlay")
	TSubclassOf<class UUserWidget> WizardOverlayClass;

	/// <summary>
	/// Widget class for the local Message
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Wizard Overlay")
	TSubclassOf<class UUserWidget> LocalMessageWidgetClass;

	/// <summary>
	/// Pointer to the gameplay overlay
	/// </summary>
	UPROPERTY()
	class UWizardOverlay* WizardOverlay;

public:
	FORCEINLINE UWizardOverlay* GetOverlay() const { return WizardOverlay; }
};
