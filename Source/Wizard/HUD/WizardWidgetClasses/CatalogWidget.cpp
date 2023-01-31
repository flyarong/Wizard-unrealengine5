// Fill out your copyright notice in the Description page of Project Settings.


#include "CatalogWidget.h"
#include "CatalogItemWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/Button.h"
#include "Wizard/Characters/WizardCharacter.h"
#include "Wizard/Components/Character/ActionComponent.h"

bool UCatalogWidget::CreateCatalog(TArray<FItemDataTable> Items)
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController) {
		AddToViewport();
		bIsFocusable = true;
		FInputModeUIOnly InputModeData;
		InputModeData.SetWidgetToFocus(TakeWidget());
		PlayerController->SetInputMode(InputModeData);

		if (Items.Num() > 0 && ItemBox && ExitButton && CatalogItemWidgetClass) {
			ExitButton->OnClicked.AddDynamic(this, &UCatalogWidget::CloseCatalog);

			for (auto& Item : Items) {
				UCatalogItemWidget* CatalogItem = CreateWidget<UCatalogItemWidget>(PlayerController, CatalogItemWidgetClass);
				if (CatalogItem) {
					CatalogItem->CreateItem(Item);
					ItemBox->AddChildToHorizontalBox(CatalogItem);
				}
			}

			return true;
		}
	}

	return false;
}

void UCatalogWidget::CloseCatalog()
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController) {
		AWizardCharacter* WCharacter = Cast<AWizardCharacter>(PlayerController->GetPawn());
		if (WCharacter && WCharacter->GetAction()) {
			RemoveFromParent();
			WCharacter->GetAction()->CloseCatalog();
			FInputModeGameAndUI InputModeData;
			PlayerController->SetInputMode(InputModeData);
		}
	}
}
