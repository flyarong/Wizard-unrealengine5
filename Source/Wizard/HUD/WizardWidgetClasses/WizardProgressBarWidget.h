// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WizardProgressBarWidget.generated.h"

/**
 * Progress Bar widget which can be used for any
 * Attribute
 */
UCLASS()
class WIZARD_API UWizardProgressBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void SetWizardBarPercent(float Value, float MaxValue);

private:

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ProgressBar;

	void UpdateWizardBarPercentage(float DeltaTime);

	/// <summary>
	/// Variable to determine how fast
	/// the progress bar for each attribute should drain/replenish
	/// </summary>
	UPROPERTY()
	float ProgressTime = 1.f;

	/// <summary>
	/// Maximum value of the progress bar
	/// </summary>
	float MaxProgressValue = 100.f;

	/// <summary>
	/// Variable to determine how much
	/// amount is to be lost/restored for
	/// the progress bar
	/// </summary>
	UPROPERTY()
	float Amount = 0.f;

	/// <summary>
	/// The rate of loss/gain
	/// </summary>
	UPROPERTY()
	float Rate = 0.f;

	/// <summary>
	/// Amount to gain/loss per frame
	/// </summary>
	UPROPERTY()
	float ValueThisFrame;

	/// <summary>
	/// Boolean for determining whether or not
	/// the progress bar is changing it's percentage
	/// </summary>
	UPROPERTY()
	bool bIsChanging = false;
};
