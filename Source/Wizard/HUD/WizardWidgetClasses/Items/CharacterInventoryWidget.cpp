// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterInventoryWidget.h"
#include "CharacterItemWidget.h"
#include "Components/Button.h"
#include "Components/UniformGridPanel.h"
#include "Wizard/Controllers/WizardPlayerController.h"

void UCharacterInventoryWidget::UpdateInventory(const TArray<FItemDataTable>& Items)
{
	ItemMap = GetItemMap(Items);
	if (ItemPanel && ItemPanel->HasAnyChildren()) ItemPanel->ClearChildren();
	if (ItemMap.Num() > 0) {
		AddItemsToInventory();
	}
	if (!ExitButton->OnClicked.IsBound()) {
		ExitButton->OnClicked.AddDynamic(this, &UCharacterInventoryWidget::CloseInventory);
	}
}

TMap<FItemDataTable, int32> UCharacterInventoryWidget::GetItemMap(const TArray<FItemDataTable>& Items)
{
	TMap<FItemDataTable, int32> TempMap;
	for (auto& Item : Items) {
		if (TempMap.Contains(Item)) {
			TempMap[Item]++;
		}
		else {
			TempMap.Add(Item, 1);
		}
	}

	return TempMap;
}

void UCharacterInventoryWidget::AddItemsToInventory()
{
	int32 Row = 0;
	int32 Column = 0;
	for (auto& Item : ItemMap) {
		if (ItemPanel && CharacterItemWidgetClass) {
			UCharacterItemWidget* CharacterItem = CreateWidget<UCharacterItemWidget>(GetOwningPlayer(), CharacterItemWidgetClass);
			if (CharacterItem) {
				CharacterItem->CreateItem(Item.Key, Item.Value);
				ItemPanel->AddChildToUniformGrid(CharacterItem, Row, Column);
				Column++;
				if (Column == NumOfItemsPerRow) {
					Row++;
					Column = 0;
				}
			}
		}
	}
}

void UCharacterInventoryWidget::CloseInventory()
{
	AWizardPlayerController* PlayerController = Cast<AWizardPlayerController>(GetOwningPlayer());
	if (PlayerController) {
		RemoveFromParent();
		PlayerController->SetInputContext(EInputContext::EIC_Default);
	}
}
