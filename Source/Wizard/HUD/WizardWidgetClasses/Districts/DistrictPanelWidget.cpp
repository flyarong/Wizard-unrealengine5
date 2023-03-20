// Fill out your copyright notice in the Description page of Project Settings.


#include "DistrictPanelWidget.h"
#include "Components/TextBlock.h"

void UDistrictPanelWidget::SetCurrentDistrictText(FText District)
{
	if (CurrentDistrictText && DistrictPanelFadeIn) {
		CurrentDistrictText->SetText(District);
		PlayAnimation(DistrictPanelFadeIn);
	}
}
