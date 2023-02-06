// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Wizard/Items/Item.h"
#include "CharacterItemPanelWidget.generated.h"

/**
 * Widget class for the Player's
 * Item Panel
 */
UCLASS()
class WIZARD_API UCharacterItemPanelWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	/// <summary>
	/// Function to add an Item to the
	/// Character Panel
	/// </summary>
	void AddCharacterItem(int32 ItemIndex);

private:

	/// <summary>
	/// Widget class for the Character item
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Character Item")
	TSubclassOf<class UUserWidget> CharacterItemWidgetClass;

	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* CharacterItemBox;

};
