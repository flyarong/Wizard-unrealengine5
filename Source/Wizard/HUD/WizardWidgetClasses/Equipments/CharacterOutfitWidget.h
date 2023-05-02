// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Wizard/Items/Item.h"
#include "Blueprint/UserWidget.h"
#include "CharacterOutfitWidget.generated.h"

/**
 * Widget class for the Character's Equipments and Outfit
 */
UCLASS()
class WIZARD_API UCharacterOutfitWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	/// <summary>
	/// Function to update the Equipments
	/// </summary>
	/// <param name="Equipments">The equipments array</param>
	void UpdateEquipments(const TArray<FItemDataTable>& Equipments);

	/// <summary>
	/// Function to update the Character Outfit
	/// </summary>
	/// <param name="Outfit">The Outfit array</param>
	void UpdateOutfit(const TArray<FItemDataTable>& Outfit);

private:

	/// <summary>
	/// Widget class for the Equipment
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Character Equipment")
	TSubclassOf<class UUserWidget> CharacterEquipmentWidgetClass;

	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* EquipmentPanel;

	UPROPERTY(meta = (BindWidget))
	class UScaleBox* GauntletBox;

	UPROPERTY(meta = (BindWidget))
	UScaleBox* BootBox;

	UPROPERTY(meta = (BindWidget))
	UScaleBox* BeltBox;

	UPROPERTY(meta = (BindWidget))
	UScaleBox* ArmorBox;

	UPROPERTY(meta = (BindWidget))
	UScaleBox* CloakBox;

	UPROPERTY(meta = (BindWidget))
	UScaleBox* StaffBox;

	UPROPERTY(meta = (BindWidget))
	class UButton* ExitButton;

	UFUNCTION()
	void CloseInventory();

	/// <summary>
	/// Variable determining the max number of Equipments
	/// per Row in the Uniform Grid
	/// </summary>
	UPROPERTY(EditAnywhere)
	int32 NumOfItemsPerRow = 5;
};
