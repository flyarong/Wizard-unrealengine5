// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMessageWidget.h"
#include "Components/VerticalBox.h"
#include "Wizard/HUD/WizardWidgetClasses/WizardOverlay.h"

void UBaseMessageWidget::AddMessageToOverlay(UWizardOverlay* Overlay)
{
	if (Overlay->GetEventBox()) {
		UVerticalBox* Box = Overlay->GetEventBox();

		if (Box->GetAllChildren().Num() >= NumOfMessages) {
			Box->GetAllChildren().RemoveAt(0); // Remove first one already if it hasn't timed out yet
		}

		Box->AddChildToVerticalBox(this);
		
		FTimerHandle MsgTimer;
		GetWorld()->GetTimerManager().SetTimer(
			MsgTimer,
			this,
			&UBaseMessageWidget::MsgTimerFinished,
			MessageTime
		);
	}
}

void UBaseMessageWidget::MsgTimerFinished()
{
	RemoveFromParent();
}
