// Fill out your copyright notice in the Description page of Project Settings.


#include "CatalogWidget.h"
#include "CatalogItemWidget.h"
#include "Components/HorizontalBox.h"

bool UCatalogWidget::CreateCatalog(TArray<FItemDataTable> Items)
{
	if (Items.Num() > 0 && ItemBox) {
		for (auto & Item : Items) {
			UCatalogItemWidget* CatalogItem = CreateWidget<UCatalogItemWidget>(GetOwningPlayer(), CatalogItemWidgetClass);
			if (CatalogItem) {
				CatalogItem->CreateCatalogItem(Item);
				ItemBox->AddChildToHorizontalBox(CatalogItem);
			}
		}

		return true;
	}

	return false;
}
