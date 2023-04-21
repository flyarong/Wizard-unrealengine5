// Fill out your copyright notice in the Description page of Project Settings.


#include "EliteEnemy.h"
#include "Wizard/GameModes/WizardGameMode.h"
#include "Wizard/Components/Actors/WizardCombatActorComponent.h"

void AEliteEnemy::BeginPlay()
{
	Super::BeginPlay();

	// Setup Elite Enemy Health & Base Damage
	UWorld* World = GetWorld();
	if (World && HasAuthority()) {
		AWizardGameMode* WGameMode = Cast<AWizardGameMode>(World->GetAuthGameMode());
		if (WGameMode && WGameMode->GetWizardPlayers().Num() > 0 && Combat) {
			int32 HealthSurplus = WGameMode->GetWizardPlayers().Num() * HealthMultiplier;
			int32 DamageSurplus = WGameMode->GetWizardPlayers().Num() * DamageMultiplier;
			Combat->AddHealth(HealthSurplus);
			Combat->AddBaseDamage(DamageSurplus);
		}
	}
}
