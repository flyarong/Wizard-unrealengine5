// Fill out your copyright notice in the Description page of Project Settings.


#include "Spell.h"
#include "Sound/SoundCue.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Wizard/Components/Actors/InteractComponent.h"
#include "Wizard/Components/MiniMap/PointOfInterestComponent.h"
#include "Wizard/Components/Actors/WizardCombatActorComponent.h"
#include "Wizard/Characters/WizardCharacter.h"
#include "Wizard/Controllers/WizardPlayerController.h"
#include "Wizard/Components/Character/ActionComponent.h"

// Sets default values
ASpell::ASpell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	// Creating the Point Of Interest Component
	POI = CreateDefaultSubobject<UPointOfInterestComponent>(TEXT("PointOfInterest"));
	POI->SetIsReplicated(true);

	// Create Combat Component
	Combat = CreateDefaultSubobject<UWizardCombatActorComponent>(TEXT("Combat"));

	// Create AreaSphere
	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(RootComponent);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	// Create BorderSphere
	BorderSphere = CreateDefaultSubobject<USphereComponent>(TEXT("BorderSphere"));
	BorderSphere->SetupAttachment(RootComponent);
	BorderSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BorderSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	// Create Interact widget
	InteractComponent = CreateDefaultSubobject<UInteractComponent>(TEXT("InteractComponent"));
	InteractComponent->SetupAttachment(RootComponent);

	// Setup Niagara Component
	SpellEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SpellEffectComponent"));
	SpellEffectComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASpell::BeginPlay()
{
	Super::BeginPlay();
	
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	AreaSphere->OnClicked.AddDynamic(this, &ASpell::OnSpellClicked);

	BorderSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

	ShowInteractWidget(false);

	Combat->SetupComponent(this, AreaSphere);
	POI->SetupPOI(this);
}

// Called every frame
void ASpell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#pragma region Interface Overrides
UTexture2D* ASpell::GetIcon()
{
	if (POI) return POI->GetIconImage();

	return nullptr;
}

void ASpell::ShowInteractWidget(bool bShowInteractWidget)
{
	if (InteractComponent) {
		InteractComponent->SetVisibility(bShowInteractWidget);
	}
}

void ASpell::SetCanInteract(bool bIsInteractable)
{
	if (Combat) Combat->SetCanInteract(bIsInteractable);
}

bool ASpell::GetCanInteract()
{
	if (Combat) return Combat->GetCanInteract();

	return false;
}

void ASpell::ReceiveDamage(int32 Damage)
{
	if (Combat) Combat->ReceiveDamage(Damage);
}

float ASpell::GetCost()
{
	if (Combat) return Combat->GetCost();

	return 0.0f;
}

int32 ASpell::GetHealth()
{
	if (Combat) return Combat->GetHealth();

	return int32();
}

void ASpell::Kill()
{
	Destroy();
}

ECombat ASpell::GetCombatType()
{
	if (Combat) return Combat->GetCombatType();

	return ECombat();
}
#pragma endregion

#pragma region Events
void ASpell::OnSpellClicked(UPrimitiveComponent* TouchedComp, FKey ButtonPressed)
{
	AWizardPlayerController* PlayerController = Cast<AWizardPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController) {
		AWizardCharacter* Character = PlayerController->GetWizardCharacter() ? PlayerController->GetWizardCharacter() :
			Cast<AWizardCharacter>(PlayerController->GetPawn());
		if (Character && Character->GetAction() && Combat && Combat->GetCanInteract() && 
			Character->GetAction()->GetOverlappedWizardActor() == this) {
			Character->GetAction()->ServerInitCombat();
		}
	}
}

void ASpell::Destroyed()
{
	AActor::Destroyed();

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
#pragma endregion