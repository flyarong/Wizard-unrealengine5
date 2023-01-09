// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ReadyButton.generated.h"

/**
 * Widget class for the players to set their
 * status to ready/not ready
 */
UCLASS()
class WIZARD_API UReadyButton : public UUserWidget
{
	GENERATED_BODY()
	
private:

	/// <summary>
	/// Ready button
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	class UButton* ReadyButton;

public:
	FORCEINLINE UButton* GetReadyButton() const { return ReadyButton; }
};
