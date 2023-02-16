// Fill out your copyright notice in the Description page of Project Settings.


#include "Spell.h"
#include "Sound/SoundCue.h"
#include "NiagaraComponent.h"
#include "Net/UnrealNetwork.h"
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

	// Setup Niagara Component
	SpellEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SpellEffectComponent"));
	SpellEffectComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASpell::BeginPlay()
{
	Super::BeginPlay();
	
	// Setup Events
	if (HasAuthority()) {
		AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &ASpell::OnSpellBeginOverlap);
		AreaSphere->OnComponentEndOverlap.AddDynamic(this, &ASpell::OnSpellEndOverlap);
	}
	AreaSphere->OnClicked.AddDynamic(this, &ASpell::OnSpellClicked);
}

void ASpell::SetSpellScore()
{

}

// Called every frame
void ASpell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#pragma region Events
void ASpell::OnSpellBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AWizardCharacter* Character = Cast<AWizardCharacter>(OtherActor);
	if (Character && Character->GetAction()) {
		Character->GetAction()->SetOverlappedSpell(this);
	}
}

void ASpell::OnSpellEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AWizardCharacter* Character = Cast<AWizardCharacter>(OtherActor);
	if (Character && Character->GetAction()) {
		Character->GetAction()->LeaveSpell();
	}
}

void ASpell::OnSpellClicked(UPrimitiveComponent* TouchedComp, FKey ButtonPressed)
{
	AWizardPlayerController* PlayerController = Cast<AWizardPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController) {
		AWizardCharacter* Character = PlayerController->GetWizardCharacter() ? PlayerController->GetWizardCharacter() :
			Cast<AWizardCharacter>(PlayerController->GetPawn());
		if (Character && Character->GetAction() &&
			Character->GetAction()->GetOverlappedSpell() == this) {
			Character->GetAction()->InitWisdomCombat();
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

	SetSpellScore();
}