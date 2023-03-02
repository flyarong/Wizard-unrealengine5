// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Wizard/Items/Item.h"
#include "CharacterInventoryWidget.generated.h"

/**
 * Widget class for the Character's Inventory
 */
UCLASS()
class WIZARD_API UCharacterInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	/// <summary>
	/// Function to update the Inventory Map
	/// </summary>
	/// <param name="Items">Character's Items array</param>
	void UpdateInventory(const TArray<FItemDataTable>& Items);

private:

	/// <summary>
	/// Map containing the Items and their amount
	/// a Character has
	/// </summary>
	UPROPERTY()
	TMap<FItemDataTable, int32> ItemMap;

	/// <summary>
	/// Function to create the ItemMap
	/// </summary>
	/// <param name="Items">Character's Items array</param>
	/// <returns>The updated ItemMap</returns>
	TMap<FItemDataTable, int32> GetItemMap(const TArray<FItemDataTable>& Items);

	/// <summary>
	/// Function to add the Items in the
	/// TMap to the Uniform Grid
	/// </summary>
	void AddItemsToInventory();

	/// <summary>
	/// Widget class for the Character item
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Character Item")
	TSubclassOf<class UUserWidget> CharacterItemWidgetClass;

	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* ItemPanel;

	UPROPERTY(meta = (BindWidget))
	class UButton* ExitButton;

	UFUNCTION()
	void CloseInventory();

	/// <summary>
	/// Variable determining the max number of Items
	/// per Row in the Uniform Grid
	/// </summary>
	UPROPERTY(EditAnywhere)
	int32 NumOfItemsPerRow = 5;
};
