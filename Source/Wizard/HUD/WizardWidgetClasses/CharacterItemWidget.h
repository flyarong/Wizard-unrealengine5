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
	void CreateItem(class AWizardCharacter* Owner, int32 Index, FItemDataTable ItemElement);

private:

	/// <summary>
	/// Item's owner
	/// </summary>
	AWizardCharacter* WCharacter;

	UPROPERTY()
	int32 ItemIndex;

	/// <summary>
	/// Item this widget is based on
	/// </summary>
	UPROPERTY()
	FItemDataTable Item;

	UPROPERTY(meta = (BindWidget))
	class UButton* ItemButton;

	UPROPERTY(meta = (BindWidget))
	class UImage* ItemImage;

	UFUNCTION()
	void OnItemButtonClicked();
};
