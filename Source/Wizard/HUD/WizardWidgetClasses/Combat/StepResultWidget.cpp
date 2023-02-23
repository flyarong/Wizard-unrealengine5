// Fill out your copyright notice in the Description page of Project Settings.


#include "StepResultWidget.h"
#include "Components/Image.h"

void UStepResultWidget::SetResult(bool bIsSuccess)
{
	if (bIsSuccess) {
		ResultImage->SetBrushFromTexture(SuccessTexture);
	}
	else {
		ResultImage->SetBrushFromTexture(FailTexture);
	}
}
