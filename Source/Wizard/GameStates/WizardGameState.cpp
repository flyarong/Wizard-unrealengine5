// Fill out your copyright notice in the Description page of Project Settings.


#include "WizardGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Wizard/Trials/Trial.h"
#include "Wizard/Enemy/EnemySpawner.h"
#include "Wizard/Enemy/Enemy.h"
#include "Wizard/Spells/Spell.h"
#include "Wizard/GameModes/WizardGameMode.h"
#include "Wizard/Controllers/WizardPlayerController.h"
#include "Wizard/Characters/WizardCharacter.h"
#include "Wizard/Interfaces/WizardActor.h"
#include "Wizard/Interfaces/WizardCombatActor.h"

void AWizardGameState::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority()) {
		Horde.Add(FName("Spell"));
		Horde.Add(FName("Spell"));
		Horde.Add(FName("Spell"));
		Horde.Add(FName("Spell"));
		Horde.Add(FName("Enemy"));
		Horde.Add(FName("Enemy"));
		Horde.Add(FName("Enemy"));
		Horde.Add(FName("Enemy"));
		Horde.Add(FName("Enemy"));
		Horde.Add(FName("Enemy"));
		Horde.Add(FName("EliteEnemy"));
		Horde.Add(FName("EliteEnemy"));
		CachedHorde = Horde;
	}
}

#pragma region WizardActors
void AWizardGameState::MoveEnemies()
{
	for (auto& CombatActor : WizardCombatActors) {
		if (CombatActor) CombatActor->MoveCombatActor();
	}
}

void AWizardGameState::StartTrial()
{
	EnemiesFinished = 0;
	WGameMode = WGameMode == nullptr ? Cast<AWizardGameMode>(GetWorld()->GetAuthGameMode()) : WGameMode;
	if (WGameMode) {
		for (auto& WizardPlayer : WGameMode->GetWizardPlayers()) {
			if (WizardPlayer && WizardPlayer->GetPawn() && TrialClass) {
				ATrial* SpawnedTrial = GetWorld()->SpawnActor<ATrial>(
					TrialClass,
					WizardPlayer->GetPawn()->GetActorLocation(),
					WizardPlayer->GetPawn()->GetActorRotation()
					);
				if (SpawnedTrial) SpawnedTrial->SetupTrial(WizardPlayer->GetPawn());
			}
		}
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
		if (MiniMapActor->ActorHasTag(FName("GoodSpell"))) GoodSpells++;
	}
}

void AWizardGameState::RemoveMiniMapActor(AActor* MiniMapActor)
{
	if (MiniMapActors.Contains(MiniMapActor)) {
		MiniMapActors.Remove(MiniMapActor);
		if (MiniMapActor->ActorHasTag(FName("WizardActor"))) RemoveWizardActor(MiniMapActor);
		if (MiniMapActor->ActorHasTag(FName("WizardCombatActor"))) RemoveCombatActor(MiniMapActor);
		if (MiniMapActor->ActorHasTag(FName("GoodSpell")) && GoodSpells > 0) GoodSpells--;
	}
}
#pragma endregion

#pragma region Enemy Spawning
void AWizardGameState::PrepareTurn()
{
	// Reset Spawnable Actors
	SpawnableEnemies = TArray<TSubclassOf<AEnemy>>();
	SpawnableSpells = TArray<TSubclassOf<ASpell>>();

	// Set Spawnable Actor classes & spawn Actors
	SetSpawnActorClasses();
	SpawnActors();

	// Check number of DarkSpells in each District
	OnDarkSpellsSpawnedDelegate.Broadcast();

	OnPrepareFinishedDelegate.ExecuteIfBound();
}

void AWizardGameState::SpawnActors()
{
	// Spawn Spells
	if (SpawnableSpells.Num() > 0) {
		for (auto& SpellClass : SpawnableSpells) {
			FVector SpawnLocation = GetSpawnLocation();
			if (SpellClass && !SpawnLocation.IsZero()) {
				GetWorld()->SpawnActor<ASpell>(
					SpellClass,
					SpawnLocation,
					FRotator()
				);
			}
		}
	}

	// Spawn Enemies
	if (SpawnableEnemies.Num() > 0) {
		for (auto& EnemyClass : SpawnableEnemies) {
			SpawnEnemy(EnemyClass);
		}
	}
}

void AWizardGameState::SpawnEnemy(TSubclassOf<AEnemy> EnemyClass)
{
	FVector SpawnLocation = GetSpawnLocation();
	if (EnemyClass && !SpawnLocation.IsZero()) {
		GetWorld()->SpawnActor<APawn>(
			EnemyClass,
			SpawnLocation,
			FRotator()
		);
	}
}

void AWizardGameState::SetSpawnActorClasses()
{
	WGameMode = WGameMode == nullptr ? Cast<AWizardGameMode>(GetWorld()->GetAuthGameMode()) : WGameMode;
	if (WGameMode && WGameMode->GetWizardPlayers().Num() > 0) {
		for (auto& WizardPlayer : WGameMode->GetWizardPlayers()) {
			for (int32 i = 0; i < NumOfActorsToSpawn; i++) {
				int32 Selection = FMath::RandRange(0, CachedHorde.Num() - 1);
				FName SpawnTag = CachedHorde[Selection];
				AddSpawnActorClass(SpawnTag);
				CachedHorde.RemoveSingle(SpawnTag);

				if (CachedHorde.Num() == 0) CachedHorde = Horde; // Refill if array empties
			}
		}
	}
}

void AWizardGameState::AddSpawnActorClass(FName Tag)
{
	if (Tag == FName("Spell")) {
		int32 Selection = GoodSpells > 0 ? 0 : FMath::RandRange(0, SpellClasses.Num() - 1);

		SpawnableSpells.Add(SpellClasses[Selection]);
	}
	else if (Tag == FName("Enemy")) {
		int32 Selection = FMath::RandRange(0, EnemyClasses.Num() - 1);

		SpawnableEnemies.Add(EnemyClasses[Selection]);
	}
	else if (Tag == FName("EliteEnemy")) {
		int32 Selection = FMath::RandRange(0, EliteEnemyClasses.Num() - 1);

		SpawnableEnemies.Add(EliteEnemyClasses[Selection]);
	}
}

FVector AWizardGameState::GetSpawnLocation()
{
	if (EnemySpawnPoints.Num() > 0) {
		int32 Selection = FMath::RandRange(0, EnemySpawnPoints.Num() - 1);

		return EnemySpawnPoints[Selection]->GetActorLocation();
	}

	return FVector::ZeroVector;
}
#pragma endregion

#pragma region StoryPoints
void AWizardGameState::PositiveOutcome()
{
	PositiveStoryPoints++;
	WGameMode = WGameMode == nullptr ? Cast<AWizardGameMode>(GetWorld()->GetAuthGameMode()) : WGameMode;
	if (WGameMode) {
		WGameMode->BroadcastStoryPointUpdate(PositiveStoryPoints);
		if (PositiveStoryPoints >= VictoryThreshold) {
			WGameMode->EndGame(true);
		}
	}
}

void AWizardGameState::NegativeOutcome()
{
	NegativeStoryPoints++;
	
	WGameMode = WGameMode == nullptr ? Cast<AWizardGameMode>(GetWorld()->GetAuthGameMode()) : WGameMode;
	if (WGameMode) {
		WGameMode->BroadcastStoryPointUpdate(NegativeStoryPoints, false);
		CheckThreshold();
	}
}

void AWizardGameState::CheckThreshold()
{
	if (NegativeStoryPoints == ((NumOfThresholdsReached + 1) * ThresholdMeter)) {
		NumOfThresholdsReached++;
		OnThresholdReached.ExecuteIfBound();

		if (NumOfThresholdsReached == 1) {
			for (int32 i = 0; i < WGameMode->GetWizardPlayers().Num(); i++) {
				int32 Selection = FMath::RandRange(0, EnemyClasses.Num() - 1);
				SpawnEnemy(EnemyClasses[Selection]);
			}
		}
		else if (NumOfThresholdsReached == 2) {
			int32 Length = 2;
			if (WGameMode->GetWizardPlayers().Num() > 2) Length = 3;
			else if (WGameMode->GetWizardPlayers().Num() > 4) Length = 4;
			for (int32 i = 0; i < Length; i++) {
				int32 Selection = FMath::RandRange(0, EliteEnemyClasses.Num() - 1);
				SpawnEnemy(EliteEnemyClasses[Selection]);
			}
		}
		else if (NumOfThresholdsReached == 3) {
			SpawnEnemy(BossClass);
		}
		else if (NumOfThresholdsReached >= 4) {
			WGameMode->EndGame(false);
		}
	}
}
#pragma endregion