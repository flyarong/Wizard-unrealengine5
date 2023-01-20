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

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ActionsText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* NumOfActionsText;

	UPROPERTY(meta = (BindWidget))
	class UMiniMapWidget* MiniMap;

public:
	FORCEINLINE void SetActionsText(FText Actions) { ActionsText->SetText(Actions); };
	FORCEINLINE UTextBlock* GetActionsText() const { return ActionsText; }
	FORCEINLINE void SetNumOfActionsText(FText NumOfActions) { NumOfActionsText->SetText(NumOfActions); };
	FORCEINLINE UTextBlock* GetNumOfActionsText() const { return NumOfActionsText; }
	FORCEINLINE void SetCurrentDistrictText(FText District) { CurrentDistrictText->SetText(District); };
	FORCEINLINE UTextBlock* GetCurrentDistrictText() const { return CurrentDistrictText; }
	FORCEINLINE UMiniMapWidget* GetMiniMap() const { return MiniMap; }
};
