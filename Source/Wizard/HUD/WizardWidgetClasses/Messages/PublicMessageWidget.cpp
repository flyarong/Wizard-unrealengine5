// Fill out your copyright notice in the Description page of Project Settings.


#include "PublicMessageWidget.h"
#include "Components/Image.h"
#include "Wizard/WizardTypes/ActionTypes.h"

void UPublicMessageWidget::AddPublicMessage(UVerticalBox* EventBox, UTexture2D* SourceTexture, EAction Event, UTexture2D* TargetTexture)
{
	SourceImage->SetBrushFromTexture(SourceTexture);
	TargetImage->SetBrushFromTexture(TargetTexture);

	switch (Event)
	{
	case EAction::EA_Movement:
		break;
	case EAction::EA_Combat:
		EventImage->SetBrushFromTexture(CombatImage);
		break;
	case EAction::EA_MAX:
		break;
	default:
		break;
	}

	AddMessageToOverlay(EventBox);
}
