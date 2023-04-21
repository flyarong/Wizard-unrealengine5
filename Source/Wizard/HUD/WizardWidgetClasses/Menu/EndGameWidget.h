// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndGameWidget.generated.h"

/**
 * Widget class for the end game screen
 */
UCLASS()
class WIZARD_API UEndGameWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	/// <summary>
	/// Function to set the End Game Widget
	/// to the screen
	/// </summary>
	/// <param name="bGameHasWon">Whether the game has been won or not</param>
	/// <param name="MenuWidget">The menu widget to add to the screen</param>
	void SetEndGameScreen(bool bGameHasWon, class UMenuWidget* MenuWidget);

private:

	UPROPERTY(meta = (BindWidget))
	class UScaleBox* MenuBox;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ResultText;
};
