// Fill out your copyright notice in the Description page of Project Settings.


#include "CurrentStepWidget.h"
#include "Components/Image.h"

void UCurrentStepWidget::ConstructSpellStep(int32 StepIndex)
{
	CurrentStepImage->SetBrushFromTexture(Symbols[StepIndex]);
}
