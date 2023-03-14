// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "Wizard/Components/Actors/InteractComponent.h"
#include "Wizard/Components/MiniMap/PointOfInterestComponent.h"
#include "Wizard/Components/Actors/WizardCombatActorComponent.h"
#include "Wizard/Characters/WizardCharacter.h"
#include "Wizard/Controllers/WizardPlayerController.h"
#include "Wizard/Components/Character/ActionComponent.h"

AEnemy::AEnemy()
{
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
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	AreaSphere->OnClicked.AddDynamic(this, &AEnemy::OnEnemyClicked);

	BorderSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

	ShowInteractWidget(false);

	Combat->SetupComponent(this, AreaSphere);
	POI->SetupPOI(this);
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

#pragma region Interface Overrides
UTexture2D* AEnemy::GetIcon()
{
	if (POI) return POI->GetIconImage();

	return nullptr;
}

void AEnemy::ShowInteractWidget(bool bShowInteractWidget)
{
	if (InteractComponent) {
		InteractComponent->SetVisibility(bShowInteractWidget);
	}
}

void AEnemy::SetCanInteract(bool bIsInteractable)
{
	if (Combat) Combat->SetCanInteract(bIsInteractable);
}

bool AEnemy::GetCanInteract()
{
	if (Combat) return Combat->GetCanInteract();

	return false;
}

void AEnemy::ReceiveDamage(int32 Damage)
{
	if (Combat) Combat->ReceiveDamage(Damage);
}

float AEnemy::GetCost()
{
	if (Combat) Combat->GetCost();

	return 0.0f;
}
int32 AEnemy::GetHealth()
{
	if (Combat) Combat->GetHealth();

	return int32();
}

void AEnemy::Kill()
{
	// TODO play death montage and sound etc.
	Destroy();
}

ECombat AEnemy::GetCombatType()
{
	if (Combat) Combat->GetCombatType();

	return ECombat();
}
#pragma endregion

#pragma region Events
void AEnemy::OnEnemyClicked(UPrimitiveComponent* TouchedComp, FKey ButtonPressed)
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
#pragma endregion
