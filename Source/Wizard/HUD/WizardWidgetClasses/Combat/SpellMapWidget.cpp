// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellMapWidget.h"
#include "Components/Image.h"

void USpellMapWidget::ConstructSpellMap(TMap<FKey, class UTexture2D*>& SpellMap)
{
	for (auto& Spell : SpellMap) {
		if (Spell.Key == EKeys::Up) {
			TopSymbol->SetBrushFromTexture(Spell.Value);
		} else if (Spell.Key == EKeys::Left) {
			LeftSymbol->SetBrushFromTexture(Spell.Value);
		} else if (Spell.Key == EKeys::Down) {
			BottomSymbol->SetBrushFromTexture(Spell.Value);
		} else if (Spell.Key == EKeys::Right) {
			RightSymbol->SetBrushFromTexture(Spell.Value);
		}
	}
}
