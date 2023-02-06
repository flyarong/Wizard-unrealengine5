// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

/**
 * Main menu class
 */
UCLASS()
class WIZARD_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	/// <summary>
	/// Setup menu for UI
	/// </summary>
	UFUNCTION(BlueprintCallable)
	virtual void MenuSetup();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USoundCue* ClickSound;

private:

	/// <summary>
	/// Fade in animation to play at startup
	/// </summary>
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* FadeInAnimation;
};
