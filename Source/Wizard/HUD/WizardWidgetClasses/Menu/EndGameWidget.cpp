// Fill out your copyright notice in the Description page of Project Settings.


#include "EndGameWidget.h"
#include "Components/TextBlock.h"
#include "Components/ScaleBox.h"
#include "MenuWidget.h"

void UEndGameWidget::SetEndGameScreen(bool bGameHasWon, UMenuWidget* MenuWidget)
{
	if (bGameHasWon) {
		ResultText->SetText(FText::FromString(TEXT("Game Won!")));
		// TODO set text color maybe
	}
	else {
		ResultText->SetText(FText::FromString(TEXT("Game Lost!")));
		// TODO set text color maybe
	}

	if (MenuBox && MenuWidget) {
		MenuWidget->SetMenu(true);
		MenuBox->AddChild(MenuWidget);
	}
}
