// Fill out your copyright notice in the Description page of Project Settings.


#include "WizardCombatActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "Components/SphereComponent.h"
#include "Wizard/Characters/WizardCharacter.h"

// Sets default values for this component's properties
UWizardCombatActorComponent::UWizardCombatActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(true);

	// ...
}

void UWizardCombatActorComponent::SetupComponent(AActor* OwningActor, USphereComponent* Sphere)
{
	Owner = OwningActor;

	// Setup Events
	if (Owner && Owner->HasAuthority() && Sphere) {
		Sphere->OnComponentBeginOverlap.AddDynamic(this, &UWizardCombatActorComponent::OnActorBeginOverlap);
		Sphere->OnComponentEndOverlap.AddDynamic(this, &UWizardCombatActorComponent::OnActorEndOverlap);
		Owner->OnDestroyed.AddDynamic(this, &UWizardCombatActorComponent::SpawnPickupItem);
	}
}

void UWizardCombatActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UWizardCombatActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWizardCombatActorComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UWizardCombatActorComponent, bCanInteract);
}

void UWizardCombatActorComponent::OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AWizardCharacter* Character = Cast<AWizardCharacter>(OtherActor);
	if (Character && Character->GetAction() && Owner) {
		Character->GetAction()->SetCurrentWizardActor(Owner);
	}
}

void UWizardCombatActorComponent::OnActorEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AWizardCharacter* Character = Cast<AWizardCharacter>(OtherActor);
	if (Character && Character->GetAction()) {
		Character->GetAction()->LeaveWizardActor();
	}
}

void UWizardCombatActorComponent::SpawnPickupItem(AActor* DestroyedActor)
{
	if (PickupClasses.Num() > 0 && NumOfSelections > 0 && Owner && Owner->HasAuthority()) {
		for (int32 i = 0; i < NumOfSelections; i++) {
			int32 Selection = FMath::RandRange(0, PickupClasses.Num() - 1);
			GetWorld()->SpawnActor<AItem>(
				PickupClasses[Selection],
				Owner->GetActorLocation() + FVector(i * 90.f, 0.f, 0.f),
				Owner->GetActorRotation()
				);
		}
	}
}

void UWizardCombatActorComponent::ReceiveDamage(int32 Damage)
{
	if (CombatType == ECombat::EC_GoodSpell) return; // No Damage to GoodSpell after Combat failed

	Health -= Damage;
}

float UWizardCombatActorComponent::GetDamage(int32 CharacterScore)
{
	return (BaseDamage - CharacterScore) * DamageAmount;
}