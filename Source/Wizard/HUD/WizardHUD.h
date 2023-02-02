// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Wizard/Items/Item.h"
#include "Wizard/WizardTypes/DistrictNames.h"
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
	/// Function to set the Energy level
	/// on the HUD
	/// </summary>
	void SetEnergy(float Energy, float MaxEnergy);

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
	/// Function to set the new Combat value
	/// on the HUD
	/// </summary>
	void SetCombat(int32 NewCombat);

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
	/// <param name="Items">Items a store can offer</param>
	void SetStoreCatalog(TArray<FItemDataTable> Items);

	/// <summary>
	/// Function to add a new item to the
	/// Character's Item Panel
	/// </summary>
	void AddCharacterItem(FItemDataTable Item);
#pragma endregion

#pragma region MiniMap
	/// <summary>
	/// Function to set the Point Of Interest on the MiniMap 
	/// on the HUD
	/// </summary>
	/// <param name="POIOwner">Point Of Interest's Owner</param>
	void SetPOIOnMiniMap(class AActor* POIOwner);
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
