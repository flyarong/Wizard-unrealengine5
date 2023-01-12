// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "WizardOverlay.generated.h"

/**
 * Overlay user widget class to show data
 * on the screen during game
 */
UCLASS()
class WIZARD_API UWizardOverlay : public UUserWidget
{
	GENERATED_BODY()
	
private:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentDistrictText;

public:
	FORCEINLINE void SetCurrentDistrictText(FText District) { CurrentDistrictText->SetText(District); };
};
