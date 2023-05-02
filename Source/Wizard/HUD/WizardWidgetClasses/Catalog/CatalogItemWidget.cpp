// Fill out your copyright notice in the Description page of Project Settings.


#include "CatalogItemWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Wizard/Characters/WizardCharacter.h"
#include "Wizard/Components/Character/ActionComponent.h"

void UCatalogItemWidget::CreateItem(const FItemDataTable& ItemRow)
{
	Item = ItemRow;

	SetItemImage();
	BoostImage->SetBrushFromTexture(Item.BoostImage);
	BoostText->SetText(FText::FromString(FString::Printf(TEXT("+%d"), Item.BoostAmount)));
	PriceText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Item.Cost)));
	BuyButton->OnClicked.AddDynamic(this, &UCatalogItemWidget::OnBuyButtonClicked);
}

void UCatalogItemWidget::OnBuyButtonClicked()
{
	AWizardCharacter* WCharacter = Cast<AWizardCharacter>(GetOwningPlayerPawn());
	if (WCharacter && WCharacter->GetAction()) {
		WCharacter->GetAction()->ServerBuyItem(Item);
	}
}

void UCatalogItemWidget::SetItemImage()
{
	switch (Item.OutfitType)
	{
	case EOutfit::EO_None:
		ItemImage->SetBrushFromTexture(Item.ItemImage);
		break;
	case EOutfit::EO_Armor:
		ItemImage->SetBrushFromTexture(ArmorTexture);
		break;
	case EOutfit::EO_Belt:
		ItemImage->SetBrushFromTexture(BeltTexture);
		break;
	case EOutfit::EO_Boot:
		ItemImage->SetBrushFromTexture(BootTexture);
		break;
	case EOutfit::EO_Cloak:
		ItemImage->SetBrushFromTexture(CloakTexture);
		break;
	case EOutfit::EO_Gauntlet:
		ItemImage->SetBrushFromTexture(GauntletTexture);
		break;
	case EOutfit::EO_Staff:
		ItemImage->SetBrushFromTexture(StaffTexture);
		break;
	default:
		break;
	}
}
