// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterItemWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Wizard/Characters/WizardCharacter.h"

void UCharacterItemWidget::CreateItem(AWizardCharacter* Owner, int32 Index, FItemDataTable ItemElement)
{
	WCharacter = Owner;

	ItemIndex = Index;
	Item = ItemElement;
	ItemImage->SetBrushFromTexture(Item.ItemImage);
	ItemButton->OnClicked.AddDynamic(this, &UCharacterItemWidget::OnItemButtonClicked);
}

void UCharacterItemWidget::OnItemButtonClicked()
{
	ItemButton->SetIsEnabled(false);
	WCharacter = WCharacter == nullptr ? Cast<AWizardCharacter>(GetOwningPlayerPawn()) : WCharacter;
	if (WCharacter) {
		//WCharacter->ServerUseItem(ItemIndex);
		RemoveFromParent();
		if (ItemButton->OnClicked.IsBound()) {
			ItemButton->OnClicked.RemoveDynamic(this, &UCharacterItemWidget::OnItemButtonClicked);
		}
	}
}
