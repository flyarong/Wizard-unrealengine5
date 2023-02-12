// Fill out your copyright notice in the Description page of Project Settings.


#include "WizardProgressBarWidget.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Styling/SlateTypes.h"
#include "Styling/SlateBrush.h"

void UWizardProgressBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateWizardBarPercentage(InDeltaTime);
}

void UWizardProgressBarWidget::SetWizardBarPercent(float Value, float MaxValue)
{
	MaxProgressValue = MaxValue;
	float Cost = -1 * (ProgressBar->GetPercent() - (Value / 100));
	Rate = Cost / ProgressTime;
	Amount += FMath::Abs(Cost);
	bIsChanging = true;
}

void UWizardProgressBarWidget::UpdateWizardBarPercentage(float DeltaTime)
{
	if (bIsChanging) {
		ValueThisFrame = Rate * DeltaTime;
		float DeltaValue = FMath::Clamp(ProgressBar->GetPercent() + ValueThisFrame, 0.f, MaxProgressValue);
		ProgressBar->SetPercent(DeltaValue);

		Amount -= FMath::Abs(ValueThisFrame);
		if (Amount <= 0.f) {
			bIsChanging = false;
			Amount = 0.f;
		}
	}
}