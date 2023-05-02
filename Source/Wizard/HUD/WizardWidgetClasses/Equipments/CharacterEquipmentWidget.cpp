// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterEquipmentWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Wizard/Characters/WizardCharacter.h"

void UCharacterEquipmentWidget::CreateEquipment(const FItemDataTable& EquipmentElement, bool bIsEquipped)
{
	Equipment = EquipmentElement;
	bEquipped = bIsEquipped;
	EquipmentImage->SetBrushFromTexture(Equipment.ItemImage);
	OutfitText->SetText(UEnum::GetDisplayValueAsText<EOutfit>(Equipment.OutfitType));
	BoostImage->SetBrushFromTexture(Equipment.BoostImage);
	BoostAmountText->SetText(FText::FromString(FString::Printf(TEXT("+%d"), Equipment.BoostAmount)));
	EquipmentButton->OnClicked.AddDynamic(this, &UCharacterEquipmentWidget::OnEquipmentButtonClicked);
}

void UCharacterEquipmentWidget::OnEquipmentButtonClicked()
{
	AWizardCharacter* WCharacter = Cast<AWizardCharacter>(GetOwningPlayerPawn());
	if (WCharacter) {
		WCharacter->PlayEquipSound();
		if (bEquipped) {
			WCharacter->ServerUnEquipOutfit(Equipment);
			RemoveFromParent();
		}
		else {
			WCharacter->ServerEquipOutfit(Equipment);
			RemoveFromParent();
		}
	}
}
