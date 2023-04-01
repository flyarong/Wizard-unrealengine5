// Fill out your copyright notice in the Description page of Project Settings.


#include "WizardCombatActor.h"

void IWizardCombatActor::ReceiveDamage(int32 Damage)
{
}

int32 IWizardCombatActor::GetBaseDamage()
{
	return 0;
}

float IWizardCombatActor::GetDamage(int32 CharacterScore)
{
	return 0.0f;
}

void IWizardCombatActor::MoveCombatActor()
{
}

void IWizardCombatActor::SetupActorForAttack()
{
}

void IWizardCombatActor::SetupActorForDefense()
{
}
