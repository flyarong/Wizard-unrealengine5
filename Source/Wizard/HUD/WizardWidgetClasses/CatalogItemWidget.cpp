// Fill out your copyright notice in the Description page of Project Settings.


#include "CatalogItemWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UCatalogItemWidget::CreateCatalogItem(FItemDataTable ItemActor)
{
	Item = ItemActor;
	ItemNameText->SetText(FText::FromString(Item.ItemName));
	ItemImage->SetBrushFromTexture(Item.ItemImage);
	BoostImage->SetBrushFromTexture(Item.BoostImage);
	BoostText->SetText(FText::FromString(FString::Printf(TEXT("+%d"), Item.BoostAmount)));
	PriceText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Item.Cost)));
}
