// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpellMapWidget.generated.h"

/**
 * Widget representing the keys and symbols for casting the Spell
 */
UCLASS()
class WIZARD_API USpellMapWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	/// <summary>
	/// Function to create the SpellMap widget
	/// </summary>
	/// <param name="SpellMap">Spell TMap to create the widget from</param>
	void ConstructSpellMap(TMap<FKey, int32>& SpellMap);

private:
	
	UPROPERTY(meta = (BindWidget))
	class UImage* TopSymbol;

	UPROPERTY(meta = (BindWidget))
	UImage* LeftSymbol;

	UPROPERTY(meta = (BindWidget))
	UImage* BottomSymbol;

	UPROPERTY(meta = (BindWidget))
	UImage* RightSymbol;

	/// <summary>
	/// Array containing the symbols presenting
	/// each Spell Step
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Spells")
	TArray<class UTexture2D*> Symbols;
};
