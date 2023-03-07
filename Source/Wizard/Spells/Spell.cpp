// Fill out your copyright notice in the Description page of Project Settings.


#include "Spell.h"
#include "Sound/SoundCue.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"
#include "Wizard/Characters/WizardCharacter.h"
#include "Wizard/Controllers/WizardPlayerController.h"
#include "Wizard/Components/Character/ActionComponent.h"
#include "Wizard/HUD/WizardWidgetClasses/OverheadWidget.h"

// Sets default values
ASpell::ASpell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	Cost = 20.f;

	// Setup Niagara Component
	SpellEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SpellEffectComponent"));
	SpellEffectComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASpell::BeginPlay()
{
	Super::BeginPlay();
	
	AreaSphere->OnClicked.AddDynamic(this, &ASpell::OnSpellClicked);
}

// Called every frame
void ASpell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#pragma region Events
void ASpell::OnSpellClicked(UPrimitiveComponent* TouchedComp, FKey ButtonPressed)
{
	AWizardPlayerController* PlayerController = Cast<AWizardPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController) {
		AWizardCharacter* Character = PlayerController->GetWizardCharacter() ? PlayerController->GetWizardCharacter() :
			Cast<AWizardCharacter>(PlayerController->GetPawn());
		if (Character && Character->GetAction() &&
			Character->GetAction()->GetOverlappedCombatActor() == this && bCanInteract) {
			Character->GetAction()->ServerInitSpellCombat();
		}
	}
}
#pragma endregion

void ASpell::Destroyed()
{
	Super::Destroyed();

	if (SpellSound) {
		UGameplayStatics::PlaySoundAtLocation(
			this,
			SpellSound,
			GetActorLocation()
		);
	}
	if (SpellEffect) {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			SpellEffect,
			GetActorLocation(),
			GetActorRotation()
		);
	}
}