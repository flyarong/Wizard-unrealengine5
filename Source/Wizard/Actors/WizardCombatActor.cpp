// Fill out your copyright notice in the Description page of Project Settings.


#include "WizardCombatActor.h"

void AWizardCombatActor::ReceiveDamage(int32 Damage)
{
	Health -= Damage;
}

void AWizardCombatActor::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority()) {
		OnDestroyed.AddDynamic(this, &AWizardCombatActor::SpawnPickupItem);
	}
}

void AWizardCombatActor::SpawnPickupItem(AActor* DestroyedActor)
{
	if (PickupClasses.Num() > 0 && NumOfSelections > 0 && HasAuthority()) {
		for (int32 i = 0; i < NumOfSelections; i++) {
			int32 Selection = FMath::RandRange(0, PickupClasses.Num() - 1);
			GetWorld()->SpawnActor<AItem>(
				PickupClasses[Selection], 
				GetActorLocation() + FVector(i * 80.f, 0.f, 0.f),
				GetActorRotation()
			);
		}
	}
}
