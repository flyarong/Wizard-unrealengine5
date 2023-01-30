// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterItemPanelWidget.h"
#include "Components/HorizontalBox.h"
#include "CharacterItemWidget.h"

void UCharacterItemPanelWidget::AddCharacterItem(FItemDataTable Item)
{
	if (CharacterItemBox && CharacterItemWidgetClass) {
		UCharacterItemWidget* CharacterItem = CreateWidget<UCharacterItemWidget>(GetOwningPlayer(), CharacterItemWidgetClass);
		if (CharacterItem) {
			CharacterItem->CreateItem(Item);
			CharacterItemBox->AddChildToHorizontalBox(CharacterItem);
		}
	}
}