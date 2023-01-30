// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Wizard/Items/Item.h"
#include "CatalogWidget.generated.h"

/**
 * Widget class for the Catalog of
 * a Store
 */
UCLASS()
class WIZARD_API UCatalogWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	/// <summary>
	/// Function to create the UI Catalog
	/// </summary>
	/// <param name="Items">Catalog Items</param>
	/// <returns>Whether the creation was successful or not</returns>
	bool CreateCatalog(TArray<FItemDataTable> Items);

private:

	/// <summary>
	/// Widget class for the CatalogItem
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Catalog")
	TSubclassOf<class UUserWidget> CatalogItemWidgetClass;

	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* ItemBox;

	UPROPERTY(meta = (BindWidget))
	class UButton* ExitButton;

	UFUNCTION()
	void CloseCatalog();
};
