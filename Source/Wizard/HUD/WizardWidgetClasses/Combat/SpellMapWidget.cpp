// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellMapWidget.h"
#include "Components/Image.h"

void USpellMapWidget::ConstructSpellMap(TMap<FKey, int32>& SpellMap)
{
	for (auto& Spell : SpellMap) {
		if (Spell.Key == EKeys::Up) {
			TopSymbol->SetBrushFromTexture(Symbols[Spell.Value]);
		} else if (Spell.Key == EKeys::Left) {
			LeftSymbol->SetBrushFromTexture(Symbols[Spell.Value]);
		} else if (Spell.Key == EKeys::Down) {
			BottomSymbol->SetBrushFromTexture(Symbols[Spell.Value]);
		} else if (Spell.Key == EKeys::Right) {
			RightSymbol->SetBrushFromTexture(Symbols[Spell.Value]);
		}
	}
}
