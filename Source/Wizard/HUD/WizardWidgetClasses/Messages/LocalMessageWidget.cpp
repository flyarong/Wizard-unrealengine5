// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMessageWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void ULocalMessageWidget::AddLocalMessage(UVerticalBox* EventBox, FText Message, EAttribute AttributeType)
{
	if (MessageText && MessageImage) {
		MessageText->SetText(Message);
		switch (AttributeType)
		{
		case EAttribute::EA_Health:
			break;
		case EAttribute::EA_Defense:
			break;
		case EAttribute::EA_Power:
			MessageImage->SetBrushFromTexture(PowerImage);
			break;
		case EAttribute::EA_XP:
			MessageImage->SetBrushFromTexture(XPImage);
			break;
		case EAttribute::EA_Wisdom:
			break;
		case EAttribute::EA_Intelligence:
			break;
		case EAttribute::EA_Offense:
			break;
		case EAttribute::EA_Agility:
			break;
		case EAttribute::EA_MAX:
			break;
		default:
			break;
		}

		AddMessageToOverlay(EventBox);
	}
}
