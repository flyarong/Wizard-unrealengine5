// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellMapWidget.h"
#include "Components/Image.h"

void USpellMapWidget::ConstructSpellMap(TMap<FKey, int32>& SpellMap)
{
	for (auto& Spell : SpellMap) {
		if (Spell.Key == EKeys::W) {
			TopSymbol->SetBrushFromTexture(Symbols[Spell.Value]);
		} else if (Spell.Key == EKeys::A) {
			LeftSymbol->SetBrushFromTexture(Symbols[Spell.Value]);
		} else if (Spell.Key == EKeys::S) {
			BottomSymbol->SetBrushFromTexture(Symbols[Spell.Value]);
		} else if (Spell.Key == EKeys::D) {
			RightSymbol->SetBrushFromTexture(Symbols[Spell.Value]);
		}
	}
}
