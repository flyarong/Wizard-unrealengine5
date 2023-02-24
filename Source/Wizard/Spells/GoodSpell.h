// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Spell.h"
#include "GoodSpell.generated.h"

/**
 * Spell class for Good Spells
 */
UCLASS()
class WIZARD_API AGoodSpell : public ASpell
{
	GENERATED_BODY()
	
public:
	AGoodSpell();

protected:
	virtual void SetSpellScore() override;
};
