// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterItemPanelWidget.h"
#include "Components/HorizontalBox.h"
#include "CharacterItemWidget.h"
#include "Wizard/Characters/WizardCharacter.h"

void UCharacterItemPanelWidget::AddCharacterItem(int32 ItemIndex)
{
	AWizardCharacter* WCharacter = Cast<AWizardCharacter>(GetOwningPlayerPawn());
	if (CharacterItemBox && CharacterItemWidgetClass && WCharacter) {
		FItemDataTable Item = WCharacter->GetLatestItem();
		UCharacterItemWidget* CharacterItem = CreateWidget<UCharacterItemWidget>(GetOwningPlayer(), CharacterItemWidgetClass);
		if (CharacterItem) {
			CharacterItem->CreateItem(WCharacter, ItemIndex, Item);
			CharacterItemBox->AddChildToHorizontalBox(CharacterItem);
		}
	}
}