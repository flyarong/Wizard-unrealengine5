// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Wizard/Items/Item.h"
#include "Blueprint/UserWidget.h"
#include "CharacterEquipmentWidget.generated.h"

/**
 * Widget class for a Character Equipment
 */
UCLASS()
class WIZARD_API UCharacterEquipmentWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	/// <summary>
	/// Function to setup the Equipment
	/// </summary>
	/// <param name="Equipment">Equipment to create</param>
	void CreateEquipment(const FItemDataTable& EquipmentElement, bool bIsEquipped = false);

private:

	/// <summary>
	/// Equipment this widget represents
	/// </summary>
	FItemDataTable Equipment;

	/// <summary>
	/// Boolean for whether or not this Equipment is
	/// equipped on the Character's outfit
	/// </summary>
	bool bEquipped = false;

	UPROPERTY(meta = (BindWidget))
	class UButton* EquipmentButton;

	UFUNCTION()
	void OnEquipmentButtonClicked();

	UPROPERTY(meta = (BindWidget))
	class UImage* EquipmentImage;

	UPROPERTY(meta = (BindWidget))
	UImage* BoostImage;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* BoostAmountText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* OutfitText;
};
