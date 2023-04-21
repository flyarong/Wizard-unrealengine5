// Fill out your copyright notice in the Description page of Project Settings.


#include "Trial.h"
#include "Sound/SoundCue.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Wizard/GameModes/WizardGameMode.h"
#include "Wizard/GameStates/WizardGameState.h"
#include "Wizard/Characters/WizardCharacter.h"
#include "Wizard/Components/Character/AttributeComponent.h"
#include "Wizard/Components/Character/ActionComponent.h"
#include "Wizard/Components/Actors/WizardCombatActorComponent.h"

// Sets default values
ATrial::ATrial()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	// Create Combat Component
	Combat = CreateDefaultSubobject<UWizardCombatActorComponent>(TEXT("Combat"));

	// Create AreaSphere
	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(RootComponent);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	// Setup Niagara Component
	TrialEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrialEffectComponent"));
	TrialEffectComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATrial::BeginPlay()
{
	Super::BeginPlay();
	
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	ShowInteractWidget(false);
	Tags.Add(FName("Trial"));
	Tags.Add(FName("WizardActor"));
	Tags.Add(FName("WizardCombatActor"));

	Combat->SetupComponent(this, AreaSphere);
}

// Called every frame
void ATrial::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATrial::Destroyed()
{
	AActor::Destroyed();

	AWizardGameMode* WGameMode = Cast<AWizardGameMode>(GetWorld()->GetAuthGameMode());
	if (WGameMode) WGameMode->IncrementPlayersFinished();

	if (TrialSound) {
		UGameplayStatics::PlaySoundAtLocation(
			this,
			TrialSound,
			GetActorLocation()
		);
	}
	if (TrialEffect) {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			TrialEffect,
			GetActorLocation(),
			GetActorRotation()
		);
	}
}

void ATrial::SetupTrial(APawn* Character)
{
	TrialCharacter = Cast<AWizardCharacter>(Character);
	if (Combat && TrialCharacter && TrialCharacter->HasAuthority() && TrialCharacter->GetAction() && TrialCharacter->GetAttribute() &&
		AreaSphere->IsOverlappingActor(TrialCharacter)) {
		bCharacterHasGoodSpell = TrialCharacter->GetAttribute()->GetGoodSpells() > 0;
		if (!bCharacterHasGoodSpell) OnDestroyed.AddDynamic(Combat, &UWizardCombatActorComponent::SpawnPickupItem);
		TrialCharacter->GetAction()->InitTrialCombat(this);
	}
}

#pragma region Interface Overrides
float ATrial::GetCost()
{
	if (Combat) return Combat->GetCost();

	return 0.0f;
}

int32 ATrial::GetHealth()
{
	if (Combat) return Combat->GetHealth();

	return int32();
}

void ATrial::Kill()
{
	// Handle Trial Success
	if (bCharacterHasGoodSpell && HasAuthority()) {
		AWizardGameState* WGameState = Cast<AWizardGameState>(UGameplayStatics::GetGameState(this));
		if (WGameState && TrialCharacter && TrialCharacter->GetAttribute()) {
			TrialCharacter->GetAttribute()->SpendGoodSpell(1);
			WGameState->PositiveOutcome();
		}
	}

	Destroy();
}

void ATrial::ReceiveDamage(int32 Damage)
{
	// Handle Trial Destroy when Trial fails
	if (OnDestroyed.IsBound()) OnDestroyed.RemoveDynamic(Combat, &UWizardCombatActorComponent::SpawnPickupItem);
	Destroy();
}

ECombat ATrial::GetCombatType()
{
	if (Combat) return Combat->GetCombatType();

	return ECombat();
}
#pragma endregion
