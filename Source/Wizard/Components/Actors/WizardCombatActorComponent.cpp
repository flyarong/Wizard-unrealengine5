// Fill out your copyright notice in the Description page of Project Settings.


#include "WizardCombatActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "Components/SphereComponent.h"
#include "Wizard/Characters/WizardCharacter.h"
#include "Wizard/Controllers/WizardPlayerController.h"
#include "Wizard/Components/Character/ActionComponent.h"

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
	AreaSphere = Sphere;
}

void UWizardCombatActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UWizardCombatActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Check Approach Radius
	if (Attacker && Owner && Owner->HasAuthority() && IsCharacterClose()) {
		Attacker->ServerInterruptMovement();
	}
}

void UWizardCombatActorComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UWizardCombatActorComponent, bCanInteract);
}

void UWizardCombatActorComponent::SetupAttackEvents()
{
	// Setup Attack Events
	if (Owner && Owner->HasAuthority() && AreaSphere) {
		if (AreaSphere->OnComponentBeginOverlap.IsBound())
			AreaSphere->OnComponentBeginOverlap.RemoveDynamic(this, &UWizardCombatActorComponent::OnActorBeginDefenseOverlap);
		if (AreaSphere->OnComponentEndOverlap.IsBound())
			AreaSphere->OnComponentEndOverlap.RemoveDynamic(this, &UWizardCombatActorComponent::OnActorEndOverlap);

		AttackedCharacter = nullptr;
		Attacker = nullptr;
	}
}

void UWizardCombatActorComponent::SetupDefendEvents()
{
	// Setup Defense Events
	if (Owner && Owner->HasAuthority() && AreaSphere) {
		AttackedCharacter = nullptr;
		Attacker = nullptr;

		if (!AreaSphere->OnComponentBeginOverlap.IsBound())
			AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &UWizardCombatActorComponent::OnActorBeginDefenseOverlap);
		if (!AreaSphere->OnComponentEndOverlap.IsBound())
			AreaSphere->OnComponentEndOverlap.AddDynamic(this, &UWizardCombatActorComponent::OnActorEndOverlap);
		if (!Owner->OnDestroyed.IsBound())
			Owner->OnDestroyed.AddDynamic(this, &UWizardCombatActorComponent::SpawnPickupItem);
	}
}

void UWizardCombatActorComponent::AttackCharacter(AActor* WCharacter)
{
	AWizardCharacter* Character = Cast<AWizardCharacter>(WCharacter);
	if (Character && Character->GetAction() && Owner && Owner->HasAuthority() && AttackedCharacter == nullptr) {
		AttackedCharacter = Character;
		if (AttackedCharacter && AttackedCharacter->GetAction()) {
			AttackedCharacter->GetAction()->OnDefenseCombatEndedDelegate.AddUObject(this, &ThisClass::OnCharacterEndedDefense);
			AttackedCharacter->GetAction()->SetAttacker(Owner);
			AttackedCharacter->GetAction()->InitDefenseCombat(Owner);
		}
	}
}

void UWizardCombatActorComponent::OnActorBeginDefenseOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AWizardCharacter* Character = Cast<AWizardCharacter>(OtherActor);
	if (Character && Character->GetAction() && Owner) {
		Character->GetAction()->SetCurrentWizardActor(Owner);
		Attacker = Character;
	}
}

void UWizardCombatActorComponent::OnActorEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AWizardCharacter* Character = Cast<AWizardCharacter>(OtherActor);
	if (Character && Character->GetAction()) {
		Character->GetAction()->LeaveWizardActor();
		Attacker = nullptr;
	}
}

void UWizardCombatActorComponent::OnCharacterEndedDefense(AWizardCharacter* CharacterAttacked, AActor* NewAttacker)
{
	if (Owner && Owner->HasAuthority() && CharacterAttacked == AttackedCharacter && NewAttacker == Owner) {
		AttackedCharacter->GetAction()->InitDefenseCombat(Owner);
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

bool UWizardCombatActorComponent::IsCharacterClose()
{
	if (Attacker == nullptr || Attacker->GetWizardController() == nullptr || Owner == nullptr) return false;

	return (Owner->GetActorLocation() - Attacker->GetActorLocation()).Size() <= ApproachRadius &&
		(Owner->GetActorLocation() - Attacker->GetWizardController()->GetCachedDestination()).Size() <= ApproachRadius;
}