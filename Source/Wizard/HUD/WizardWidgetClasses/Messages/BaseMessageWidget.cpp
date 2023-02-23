// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMessageWidget.h"
#include "Components/VerticalBox.h"

void UBaseMessageWidget::AddMessageToOverlay(UVerticalBox* EventBox)
{
	if (EventBox) {
		if (EventBox->GetAllChildren().Num() >= NumOfMessages) {
			EventBox->GetAllChildren()[0]->RemoveFromParent(); // Remove first one already if it hasn't timed out yet
		}

		EventBox->AddChildToVerticalBox(this);
		
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
