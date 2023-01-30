// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemWidget.h"
#include "Components/Image.h"
#include "CharacterItemWidget.generated.h"

/**
 * Widget class for an item in
 * the Character's items
 */
UCLASS()
class WIZARD_API UCharacterItemWidget : public UItemWidget
{
	GENERATED_BODY()
	
public:

	/// <summary>
	/// Function to create the Character Item widget
	/// </summary>
	virtual void CreateItem(FItemDataTable ItemRow) override;

private:

	UPROPERTY(meta = (BindWidget))
	class UImage* ItemImage;

public:

	FORCEINLINE void SetItemImage(class UTexture2D* Image) { if (ItemImage) ItemImage->SetBrushFromTexture(Image); }
};
