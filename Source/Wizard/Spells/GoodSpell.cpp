// Fill out your copyright notice in the Description page of Project Settings.


#include "GoodSpell.h"

AGoodSpell::AGoodSpell()
{
	Cost = 40.f;
	Health = 2;
}

void AGoodSpell::DamageActor(int32 Damage)
{
	// No Damage to GoodSpell after Combat failed
}
