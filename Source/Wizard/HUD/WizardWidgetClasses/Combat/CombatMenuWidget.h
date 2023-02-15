// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CombatMenuWidget.generated.h"

/**
 * Widget class for the Combat Menu
 */
UCLASS()
class WIZARD_API UCombatMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void BindEventsToButtons();

private:

	UPROPERTY(meta = (BindWidget))
	class UButton* StartButton;

	UPROPERTY(meta = (BindWidget))
	UButton* CancelButton;

	void OnStartButtonClicked();

	void OnCancelButtonClicked();

};
