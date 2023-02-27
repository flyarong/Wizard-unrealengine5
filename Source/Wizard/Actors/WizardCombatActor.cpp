// Fill out your copyright notice in the Description page of Project Settings.


#include "WizardCombatActor.h"

void AWizardCombatActor::DamageActor(int32 Damage)
{
	Health -= Damage;
}
