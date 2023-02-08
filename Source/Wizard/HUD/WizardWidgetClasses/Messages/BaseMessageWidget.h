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

	void AddMessageToOverlay(class UWizardOverlay* Overlay);

	UPROPERTY(EditAnywhere)
	int32 NumOfMessages = 5;

	UPROPERTY()
	float MessageTime = 5.f;

	void MsgTimerFinished();
};
