// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterOutfitWidget.h"
#include "CharacterEquipmentWidget.h"
#include "Components/Button.h"
#include "Components/UniformGridPanel.h"
#include "Components/ScaleBox.h"

void UCharacterOutfitWidget::UpdateEquipments(const TArray<FItemDataTable>& Equipments)
{
	if (EquipmentPanel && EquipmentPanel->HasAnyChildren()) EquipmentPanel->ClearChildren();
	int32 Row = 0;
	int32 Column = 0;
	for (auto& Equipment : Equipments) {
		if (EquipmentPanel && CharacterEquipmentWidgetClass) {
			UCharacterEquipmentWidget* EquipmentWidget = CreateWidget<UCharacterEquipmentWidget>(GetOwningPlayer(), CharacterEquipmentWidgetClass);
			if (EquipmentWidget) {
				EquipmentWidget->CreateEquipment(Equipment);
				EquipmentPanel->AddChildToUniformGrid(EquipmentWidget, Row, Column);
				Column++;
				if (Column == NumOfItemsPerRow) {
					Row++;
					Column = 0;
				}
			}
		}
	}

	if (!ExitButton->OnClicked.IsBound()) {
		ExitButton->OnClicked.AddDynamic(this, &UCharacterOutfitWidget::CloseInventory);
	}
}

void UCharacterOutfitWidget::UpdateOutfit(const TArray<FItemDataTable>& Outfit)
{
	for (auto& Equipment : Outfit) {
		if (EquipmentPanel && CharacterEquipmentWidgetClass) {
			UCharacterEquipmentWidget* EquipmentWidget = CreateWidget<UCharacterEquipmentWidget>(GetOwningPlayer(), CharacterEquipmentWidgetClass);
			if (EquipmentWidget) {
				EquipmentWidget->CreateEquipment(Equipment, true);
				switch (Equipment.OutfitType)
				{
				case EOutfit::EO_Gauntlet:
					if (GauntletBox && GauntletBox->HasAnyChildren()) GauntletBox->ClearChildren();
					GauntletBox->AddChild(EquipmentWidget);
					break;
				case EOutfit::EO_Boot:
					if (BootBox && BootBox->HasAnyChildren()) BootBox->ClearChildren();
					BootBox->AddChild(EquipmentWidget);
					break;
				case EOutfit::EO_Belt:
					if (BeltBox && BeltBox->HasAnyChildren()) BeltBox->ClearChildren();
					BeltBox->AddChild(EquipmentWidget);
					break;
				case EOutfit::EO_Armor:
					if (ArmorBox && ArmorBox->HasAnyChildren()) ArmorBox->ClearChildren();
					ArmorBox->AddChild(EquipmentWidget);
					break;
				case EOutfit::EO_Cloak:
					if (CloakBox && CloakBox->HasAnyChildren()) CloakBox->ClearChildren();
					CloakBox->AddChild(EquipmentWidget);
					break;
				case EOutfit::EO_Staff:
					if (StaffBox && StaffBox->HasAnyChildren()) StaffBox->ClearChildren();
					StaffBox->AddChild(EquipmentWidget);
					break;
				default:
					break;
				}
			}
		}
	}

	if (!ExitButton->OnClicked.IsBound()) {
		ExitButton->OnClicked.AddDynamic(this, &UCharacterOutfitWidget::CloseInventory);
	}
}

void UCharacterOutfitWidget::CloseInventory()
{
	RemoveFromParent();
}
