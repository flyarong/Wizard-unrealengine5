// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CurrentStepWidget.generated.h"

/**
 * Widget class for the current Spell Step
 */
UCLASS()
class WIZARD_API UCurrentStepWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void ConstructSpellStep(int32 StepIndex);

private:

	UPROPERTY(meta = (BindWidget))
	class UImage* CurrentStepImage;

	/// <summary>
	/// Array containing the symbols presenting
	/// each spell step
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Spells")
	TArray<class UTexture2D*> Symbols;

};
