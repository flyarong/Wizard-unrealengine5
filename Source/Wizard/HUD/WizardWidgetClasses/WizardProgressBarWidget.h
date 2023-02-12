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
	
	/// <summary>
	/// Function to set the percentage of
	/// the progress bar
	/// Value / MaxValue
	/// </summary>
	/// <param name="Value">New Value</param>
	/// <param name="MaxValue">Maximum Value</param>
	void SetWizardBarPercent(float Value, float MaxValue);

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* ProgressBar;
	
	/// <summary>
	/// Size X of the Progress Bar
	/// </summary>
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ProgressBar Settings")
	float GridSizeX = 280.f;

	/// <summary>
	/// Size Y of the Progress Bar
	/// </summary>
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ProgressBar Settings")
	float GridSizeY = 50.f;

	/// <summary>
	/// Icon next to the progress bar
	/// </summary>
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ProgressBar Settings")
	class UTexture2D* ProgressBarIcon;

	/// <summary>
	/// Whether the ProgressBar Icon should be
	/// rounded or not
	/// </summary>
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ProgressBar Settings")
	bool bIconIsRounded = false;

	/// <summary>
	/// Color of the progress bar
	/// </summary>
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ProgressBar Settings")
	UTexture2D* ProgressBarForeground;

private:

	/// <summary>
	/// Function to animate the percentage
	/// update
	/// </summary>
	/// <param name="DeltaTime">Delta time from tick event</param>
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
	UPROPERTY()
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
	float ValueThisFrame = 0.f;

	/// <summary>
	/// Boolean for determining whether or not
	/// the progress bar is changing it's percentage
	/// </summary>
	UPROPERTY()
	bool bIsChanging = false;
};
