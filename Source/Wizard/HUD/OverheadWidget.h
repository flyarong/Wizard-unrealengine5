// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverheadWidget.generated.h"

/**
 * Widget class for showing the player's name
 * on the character
 */
UCLASS()
class WIZARD_API UOverheadWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeDestruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DisplayText;
	
public:
	void SetDisplayText(FString TextToDisplay);
};
