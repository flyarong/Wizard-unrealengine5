// Fill out your copyright notice in the Description page of Project Settings.


#include "CatalogItemWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Wizard/Items/Item.h"
#include "Wizard/Characters/WizardCharacter.h"
#include "Wizard/Components/Character/ActionComponent.h"

void UCatalogItemWidget::CreateItem(FItemDataTable ItemRow)
{
	Super::CreateItem(ItemRow);

	ItemNameText->SetText(FText::FromString(Item.ItemName));
	ItemImage->SetBrushFromTexture(Item.ItemImage);
	BoostImage->SetBrushFromTexture(Item.BoostImage);
	BoostText->SetText(FText::FromString(FString::Printf(TEXT("+%d"), Item.BoostAmount)));
	PriceText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Item.Cost)));
	BuyButton->OnClicked.AddDynamic(this, &UCatalogItemWidget::BuyItem);
}

void UCatalogItemWidget::BuyItem()
{
	AWizardCharacter* WCharacter = Cast<AWizardCharacter>(GetOwningPlayerPawn());
	if (WCharacter && WCharacter->GetAction()) {
		WCharacter->GetAction()->BuyItem(Item);
	}
}
