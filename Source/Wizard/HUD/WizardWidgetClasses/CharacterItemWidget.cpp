// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterItemWidget.h"

void UCharacterItemWidget::CreateItem(FItemDataTable ItemRow)
{
	Super::CreateItem(ItemRow);

	ItemImage->SetBrushFromTexture(Item.ItemImage);
}