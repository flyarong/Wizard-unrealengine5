// Fill out your copyright notice in the Description page of Project Settings.


#include "CatalogWidget.h"
#include "CatalogItemWidget.h"
#include "Components/UniformGridPanel.h"
#include "Wizard/Items/Item.h"
#include "Wizard/Stores/Store.h"
#include "Components/Button.h"
#include "Wizard/Characters/WizardCharacter.h"
#include "Wizard/Controllers/WizardPlayerController.h"
#include "Wizard/Components/Character/ActionComponent.h"

bool UCatalogWidget::CreateCatalog(AStore* Store)
{
	AWizardPlayerController* PlayerController = Cast<AWizardPlayerController>(GetOwningPlayer());
	if (PlayerController) {
		PlayerController->SetWizardMovementIsEnabled(false);
		AddToViewport();
		bIsFocusable = true;

		if (Store->GetStoreCatalog().Num() > 0 && ItemPanel && ExitButton && CatalogItemWidgetClass) {
			ExitButton->OnClicked.AddDynamic(this, &UCatalogWidget::CloseCatalog);

			int32 Column = 0;
			for (auto& Item : Store->GetStoreCatalog()) {
				UCatalogItemWidget* CatalogItem = CreateWidget<UCatalogItemWidget>(PlayerController, CatalogItemWidgetClass);
				if (CatalogItem) {
					CatalogItem->CreateItem(Item.Key, Item.Value);
					ItemPanel->AddChildToUniformGrid(CatalogItem, 0, Column);
					Column++;
				}
			}

			return true;
		}
	}

	return false;
}

void UCatalogWidget::CloseCatalog()
{
	AWizardPlayerController* PlayerController = Cast<AWizardPlayerController>(GetOwningPlayer());
	if (PlayerController) {
		AWizardCharacter* WCharacter = Cast<AWizardCharacter>(PlayerController->GetPawn());
		if (WCharacter && WCharacter->GetAction()) {
			RemoveFromParent();
			WCharacter->GetAction()->CloseCatalog();
			PlayerController->SetWizardMovementIsEnabled(true);
		}
	}
}
