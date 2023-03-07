// Fill out your copyright notice in the Description page of Project Settings.


#include "WizardCombatActor.h"
#include "Net/UnrealNetwork.h"
#include "Components/SphereComponent.h"
#include "Wizard/Characters/WizardCharacter.h"
#include "Wizard/Components/Character/ActionComponent.h"

void AWizardCombatActor::ReceiveDamage(int32 Damage)
{
	Health -= Damage;
}

void AWizardCombatActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWizardCombatActor, bCanInteract);
}

void AWizardCombatActor::BeginPlay()
{
	Super::BeginPlay();

	// Setup Events
	if (HasAuthority()) {
		AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AWizardCombatActor::OnActorBeginOverlap);
		AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AWizardCombatActor::OnActorEndOverlap);
		OnDestroyed.AddDynamic(this, &AWizardCombatActor::SpawnPickupItem);
	}
}

void AWizardCombatActor::OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AWizardCharacter* Character = Cast<AWizardCharacter>(OtherActor);
	if (Character && Character->GetAction()) {
		Character->GetAction()->SetOverlappedCombatActor(this);
	}
}

void AWizardCombatActor::OnActorEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AWizardCharacter* Character = Cast<AWizardCharacter>(OtherActor);
	if (Character && Character->GetAction()) {
		Character->GetAction()->LeaveCombatActor();
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

void AWizardCombatActor::SetCanInteract(bool bIsInteractable)
{
	bCanInteract = bIsInteractable;
}
