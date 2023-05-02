// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Wizard/Items/Item.h"
#include "Blueprint/UserWidget.h"
#include "CatalogItemWidget.generated.h"

/**
 * Widget class for an Item
 * in the Catalog
 */
UCLASS()
class WIZARD_API UCatalogItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	/// <summary>
	/// Function to create an Item on the Catalog
	/// </summary>
	/// <param name="ItemRow">DataTable Row to create the widget from</param>
	void CreateItem(const FItemDataTable& ItemRow);

private:

	/// <summary>
	/// Item this widget is based on
	/// </summary>
	UPROPERTY()
	FItemDataTable Item;

	UPROPERTY(meta = (BindWidget))
	class UImage* ItemImage;

	UPROPERTY(meta = (BindWidget))
	UImage* BoostImage;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* BoostText;

	UPROPERTY(meta = (BindWidget))
	UImage* PriceImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PriceText;

	UPROPERTY(meta = (BindWidget))
	class UButton* BuyButton;

	UFUNCTION()
	void OnBuyButtonClicked();

	UPROPERTY(EditAnywhere, Category = "Outfits")
	class UTexture2D* ArmorTexture;

	UPROPERTY(EditAnywhere, Category = "Outfits")
	UTexture2D* CloakTexture;

	UPROPERTY(EditAnywhere, Category = "Outfits")
	UTexture2D* GauntletTexture;

	UPROPERTY(EditAnywhere, Category = "Outfits")
	UTexture2D* StaffTexture;

	UPROPERTY(EditAnywhere, Category = "Outfits")
	UTexture2D* BeltTexture;

	UPROPERTY(EditAnywhere, Category = "Outfits")
	UTexture2D* BootTexture;

	void SetItemImage();
};
