// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseMessageWidget.generated.h"

/**
 * Base class for implementing HUD messages
 */
UCLASS()
class WIZARD_API UBaseMessageWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	/// <summary>
	/// Function to add a new message to the
	/// overlay
	/// </summary>
	void AddMessageToOverlay(class UWizardOverlay* Overlay);

	/// <summary>
	/// Number of messages to show in the messagebox
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Message")
	int32 NumOfMessages = 4;

	/// <summary>
	/// Number of seconds to show the message
	/// </summary>
	UPROPERTY()
	float MessageTime = 5.f;

	/// <summary>
	/// Callback function to the message's timer
	/// </summary>
	void MsgTimerFinished();
};
