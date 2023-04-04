// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MatchStateWidget.generated.h"

/**
 * Widget class for showing the current MatchState to Players
 */
UCLASS()
class WIZARD_API UMatchStateWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	/// <summary>
	/// Function to set the current MatchState
	/// in the widget
	/// </summary>
	/// <param name="StateOfMatch">current MatchState</param>
	void SetMatchState(const FName& StateOfMatch);

private:

	/// <summary>
	/// Player controller pointer
	/// </summary>
	class AWizardPlayerController* PlayerController;

	/// <summary>
	/// Variable holding the current MatchState
	/// </summary>
	FName CurrentState;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MatchStateText;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* MatchStateFade;

	FWidgetAnimationDynamicEvent EndDelegate;

	/// <summary>
	/// Callback function to when the Fade animation
	/// ends
	/// </summary>
	UFUNCTION()
	void OnMatchStateAnimationFinished();
};
