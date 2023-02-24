// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseMessageWidget.h"
#include "Wizard/WizardTypes/ActionTypes.h"
#include "PublicMessageWidget.generated.h"

/**
 * Message class for public events
 */
UCLASS()
class WIZARD_API UPublicMessageWidget : public UBaseMessageWidget
{
	GENERATED_BODY()
	
public:

	void AddPublicMessage(class UVerticalBox* EventBox, class UTexture2D* SourceTexture, EAction Event, UTexture2D* TargetTexture);

private:

	UPROPERTY(meta = (BindWidget))
	class UImage* SourceImage;

	UPROPERTY(meta = (BindWidget))
	UImage* EventImage;

	UPROPERTY(meta = (BindWidget))
	UImage* TargetImage;

	/// <summary>
	/// Combat Image to show Combat Event
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Event Icons")
	class UTexture2D* CombatImage;
};
