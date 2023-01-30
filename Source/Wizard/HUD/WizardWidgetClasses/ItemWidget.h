// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Wizard/Items/Item.h"
#include "ItemWidget.generated.h"

/**
 * Base class for the Item widgets
 */
UCLASS()
class WIZARD_API UItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	/// <summary>
	/// Virtual function to create the item widget
	/// </summary>
	virtual void CreateItem(FItemDataTable ItemRow);

protected:
	/// <summary>
	/// Item this widget is based on
	/// </summary>
	UPROPERTY()
	FItemDataTable Item;

};
