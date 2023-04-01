// Fill out your copyright notice in the Description page of Project Settings.


#include "WizardGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Wizard/Trials/Trial.h"
#include "Wizard/Characters/WizardCharacter.h"
#include "Wizard/Interfaces/WizardActor.h"
#include "Wizard/Interfaces/WizardCombatActor.h"

void AWizardGameState::BeginPlay()
{
	Super::BeginPlay();

}

#pragma region WizardActors
void AWizardGameState::MoveEnemies()
{
	for (auto& CombatActor : WizardCombatActors) {
		if (CombatActor) CombatActor->MoveCombatActor();
	}
}

void AWizardGameState::StartTrial(APawn* Character)
{
	if (Character && TrialClass) {
		ATrial* SpawnedTrial = GetWorld()->SpawnActor<ATrial>(
			TrialClass,
			Character->GetActorLocation(),
			Character->GetActorRotation()
		);
		if (SpawnedTrial) SpawnedTrial->SetupTrial(Character);
	}
}

void AWizardGameState::EnableWizardActors()
{
	for (auto& WizardActor : WizardActors) {
		if (WizardActor) {
			WizardActor->SetCanInteract(true);
		}
	}

	for (auto& CombatActor : WizardCombatActors) {
		if (CombatActor) {
			CombatActor->SetupActorForDefense();
		}
	}
}

void AWizardGameState::DisableWizardActors()
{
	EnemiesFinished = 0;
	for (auto& WizardActor : WizardActors) {
		if (WizardActor) {
			WizardActor->SetCanInteract(false);
		}
	}

	for (auto& CombatActor : WizardCombatActors) {
		if (CombatActor) {
			CombatActor->SetupActorForAttack();
		}
	}
}

void AWizardGameState::AddWizardActor(AActor* WizardActor)
{
	IWizardActor* WActor = Cast<IWizardActor>(WizardActor);
	if (WActor) WizardActors.Add(WActor);
}

void AWizardGameState::RemoveWizardActor(AActor* WizardActor)
{
	IWizardActor* WActor = Cast<IWizardActor>(WizardActor);
	if (WActor) WizardActors.RemoveSingle(WActor);
}

void AWizardGameState::AddCombatActor(AActor* CombatActor)
{
	IWizardCombatActor* WCombatActor = Cast<IWizardCombatActor>(CombatActor);
	if (WCombatActor) WizardCombatActors.Add(WCombatActor);
}

void AWizardGameState::RemoveCombatActor(AActor* CombatActor)
{
	IWizardCombatActor* WCombatActor = Cast<IWizardCombatActor>(CombatActor);
	if (WCombatActor) WizardCombatActors.RemoveSingle(WCombatActor);
}
#pragma endregion

#pragma region MiniMap
void AWizardGameState::AddMiniMapActor(AActor* MiniMapActor)
{
	if (MiniMapActor) {
		MiniMapActors.AddUnique(MiniMapActor);
		if (MiniMapActor->ActorHasTag(FName("WizardActor"))) AddWizardActor(MiniMapActor);
		if (MiniMapActor->ActorHasTag(FName("WizardCombatActor"))) AddCombatActor(MiniMapActor);
	}
}

void AWizardGameState::RemoveMiniMapActor(AActor* MiniMapActor)
{
	if (MiniMapActors.Contains(MiniMapActor)) {
		MiniMapActors.Remove(MiniMapActor);
		if (MiniMapActor->ActorHasTag(FName("WizardActor"))) RemoveWizardActor(MiniMapActor);
		if (MiniMapActor->ActorHasTag(FName("WizardCombatActor"))) RemoveCombatActor(MiniMapActor);
	}
}
#pragma endregion