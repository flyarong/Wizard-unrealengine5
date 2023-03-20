// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "Wizard/Components/Actors/InteractComponent.h"
#include "Wizard/Components/MiniMap/PointOfInterestComponent.h"
#include "Wizard/Components/Actors/WizardCombatActorComponent.h"
#include "Wizard/Characters/WizardCharacter.h"
#include "Wizard/Controllers/WizardPlayerController.h"
#include "AIController.h"
#include "Wizard/Components/Character/ActionComponent.h"
#include "Wizard/GameModes/WizardGameMode.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

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

	// Create Pawn Sensing
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 500.f;
	PawnSensing->SetPeripheralVisionAngle(45.f);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
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
	POI->ServerSetupPOI(this);

	if (HasAuthority()) {
		ChosenAttributeToChase = GetChosenAttribute();
	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

EAttribute AEnemy::GetChosenAttribute()
{
	UEnum* AttrEnum = StaticEnum<EAttribute>();
	if (AttrEnum) {
		int32 EnumIndex = FMath::RandRange(0, int32(AttrEnum->GetMaxEnumValue() - 1));

		return EAttribute(AttrEnum->GetValueByIndex(EnumIndex));
	}

	return EAttribute();
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
	// TODO hit animation & sound
}

int32 AEnemy::GetBaseDamage()
{
	if (Combat) return Combat->GetBaseDamage();

	return int32();
}

float AEnemy::GetDamage(int32 CharacterScore)
{
	if (Combat) return Combat->GetDamage(CharacterScore);

	return 0.0f;
}

float AEnemy::GetCost()
{
	if (Combat) return Combat->GetCost();

	return 0.0f;
}
int32 AEnemy::GetHealth()
{
	if (Combat) return Combat->GetHealth();

	return int32();
}

void AEnemy::Kill()
{
	// TODO play death montage and sound etc.
	Destroy();
}

ECombat AEnemy::GetCombatType()
{
	if (Combat) return Combat->GetCombatType();

	return ECombat();
}

void AEnemy::MoveCombatActor()
{
	if (HasAuthority()) {
		if (PawnSensing && !PawnSensing->OnSeePawn.IsBound()) PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::OnSeeWizard);
		GetWorld()->GetTimerManager().SetTimer(
			SensingTimer,
			this,
			&AEnemy::MoveEnemyToUnseenWizard,
			1.f
		);
	}
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

void AEnemy::OnSeeWizard(APawn* Pawn)
{
	if (Pawn->ActorHasTag(FName("WizardCharacter"))) {
		GetWorld()->GetTimerManager().ClearTimer(SensingTimer);
		MoveToWizard(Pawn);
	}
}
#pragma endregion

#pragma region Movement
void AEnemy::MoveEnemyToUnseenWizard()
{
	AWizardGameMode* WGameMode = Cast<AWizardGameMode>(GetWorld()->GetAuthGameMode());
	if (WGameMode) {
		AWizardCharacter* TargetCharacter = WGameMode->GetCharacterWithLowestAttribute(ChosenAttributeToChase);
		if (TargetCharacter) {
			MoveToWizard(TargetCharacter);
			FTimerHandle MovementTimer;
			GetWorld()->GetTimerManager().SetTimer(
				MovementTimer,
				this,
				&AEnemy::StopEnemyMovement,
				MovementTime
			);
		}
	}
}

void AEnemy::MoveToWizard(AActor* TargetWizard)
{
	EnemyController = EnemyController == nullptr ? Cast<AAIController>(Controller) : EnemyController;
	if (EnemyController && TargetWizard && TargetWizard->ActorHasTag(FName("WizardCharacter"))) {
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(TargetWizard);
		MoveRequest.SetAcceptanceRadius(BorderSphere->GetUnscaledSphereRadius());
		EnemyController->MoveTo(MoveRequest);
	}
}

void AEnemy::StopEnemyMovement()
{
	EnemyController = EnemyController == nullptr ? Cast<AAIController>(Controller) : EnemyController;
	if (EnemyController) {
		EnemyController->StopMovement();
	}
}
#pragma endregion
