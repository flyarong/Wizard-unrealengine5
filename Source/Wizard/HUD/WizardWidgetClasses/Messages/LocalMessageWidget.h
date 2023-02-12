// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Wizard/WizardTypes/AttributeTypes.h"
#include "BaseMessageWidget.h"
#include "LocalMessageWidget.generated.h"

/**
 * Widget class for local Event messages
 */
UCLASS()
class WIZARD_API ULocalMessageWidget : public UBaseMessageWidget
{
	GENERATED_BODY()
	
public:

	void AddLocalMessage(class UWizardOverlay* Overlay, class FText MessageText, EAttribute AttributeType);

private:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MessageText;

	UPROPERTY(meta = (BindWidget))
	class UImage* MessageImage;

	/// <summary>
	/// XP image to show next to the message
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Message")
	class UTexture2D* XPImage;
};
