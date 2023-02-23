// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "CombatScoreWidget.generated.h"

/**
 * Widget class to show the Combat Score
 */
UCLASS()
class WIZARD_API UCombatScoreWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CombatScoreText;

public:
	FORCEINLINE void SetCombatScore(const FText& Score) { if (CombatScoreText) CombatScoreText->SetText(Score); }
};
