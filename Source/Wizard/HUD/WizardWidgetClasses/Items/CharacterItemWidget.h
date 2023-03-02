// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Wizard/Items/Item.h"
#include "Blueprint/UserWidget.h"
#include "CharacterItemWidget.generated.h"

/**
 * Widget class for an item in
 * the Character's items
 */
UCLASS()
class WIZARD_API UCharacterItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	/// <summary>
	/// Function to create the Character Item widget
	/// </summary>
	/// <param name="ItemElement">Item to create the Character Item from</param>
	/// <param name="Count">Count of this particular Item</param>
	void CreateItem(const FItemDataTable& ItemElement, int32 Count);

private:

	/// <summary>
	/// Item this widget is based on
	/// </summary>
	UPROPERTY()
	FItemDataTable Item;

	/// <summary>
	/// Count of this particular Item
	/// </summary>
	UPROPERTY()
	int32 ItemCount = 0;

	UPROPERTY(meta = (BindWidget))
	class UButton* ItemButton;

	UFUNCTION()
	void OnItemButtonClicked();

	UPROPERTY(meta = (BindWidget))
	class UImage* ItemImage;

	UPROPERTY(meta = (BindWidget))
	UImage* BoostImage;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* BoostAmountText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* QuantityText;

};
