// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatMessageWidget.generated.h"

/**
 * Widget class for the Chatbox message
 */
UCLASS()
class WIZARD_API UChatMessageWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void ConstructMessage(const FText& Message);

private:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MessageText;

};
