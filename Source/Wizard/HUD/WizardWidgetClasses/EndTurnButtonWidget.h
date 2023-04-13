// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndTurnButtonWidget.generated.h"

/**
 * Button Widget to end the Player's current turn
 */
UCLASS()
class WIZARD_API UEndTurnButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	void SetIsButtonEnabled(bool bEnabled);

private:

	UPROPERTY(meta = (BindWidget))
	class UButton* EndTurnButton;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* EndTurnText;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	class USoundCue* ClickSound;

	/// <summary>
	/// Callback function to the button's
	/// OnClicked event
	/// </summary>
	UFUNCTION()
	void OnEndTurnButtonClicked();

	/// <summary>
	/// Boolean for whether or not the Player is
	/// in their turn
	/// </summary>
	UPROPERTY()
	bool bPlayerInTurn = false;
};
