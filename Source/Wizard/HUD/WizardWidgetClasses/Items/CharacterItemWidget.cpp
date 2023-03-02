// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterItemWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Wizard/Characters/WizardCharacter.h"

void UCharacterItemWidget::CreateItem(const FItemDataTable& ItemElement, int32 Count)
{
	Item = ItemElement;
	ItemCount = Count;
	ItemImage->SetBrushFromTexture(Item.ItemImage);
	BoostImage->SetBrushFromTexture(Item.BoostImage);
	BoostAmountText->SetText(FText::FromString(FString::Printf(TEXT("+%d"), Item.BoostAmount)));
	QuantityText->SetText(FText::FromString(FString::Printf(TEXT("%d"), ItemCount)));
	ItemButton->OnClicked.AddDynamic(this, &UCharacterItemWidget::OnItemButtonClicked);
}

void UCharacterItemWidget::OnItemButtonClicked()
{
	ItemButton->SetIsEnabled(false);
	AWizardCharacter* WCharacter = Cast<AWizardCharacter>(GetOwningPlayerPawn());
	if (WCharacter) {
		WCharacter->PlayUseSound();
		WCharacter->ServerUseItem(Item);
	}
}
