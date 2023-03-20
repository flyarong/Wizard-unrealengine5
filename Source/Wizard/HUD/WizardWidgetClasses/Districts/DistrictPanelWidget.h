// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DistrictPanelWidget.generated.h"

/**
 * Widget class to show the current District's name
 */
UCLASS()
class WIZARD_API UDistrictPanelWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	/// <summary>
	/// Function to set the current District's
	/// name as Panel Text
	/// </summary>
	/// <param name="District">The current District's name</param>
	void SetCurrentDistrictText(FText District);

private:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentDistrictText;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* DistrictPanelFadeIn;

};
