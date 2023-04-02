// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
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

	// Create Interact widget
	InteractComponent = CreateDefaultSubobject<UInteractComponent>(TEXT("InteractComponent"));
	InteractComponent->SetupAttachment(RootComponent);

	// Create Pawn Sensing
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 1250.f;
	PawnSensing->SetPeripheralVisionAngle(50.f);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetCapsuleComponent()->OnClicked.AddDynamic(this, &AEnemy::OnEnemyClicked);

	ShowInteractWidget(false);
	Tags.Add(FName("Enemy"));
	Tags.Add(FName("WizardActor"));
	Tags.Add(FName("WizardCombatActor"));

	Combat->SetupComponent(this, AreaSphere);
	POI->ServerSetupPOI(this);

	if (HasAuthority()) {
		ChosenAttributeToChase = GetChosenAttribute();
	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Combat && TargetCharacter && InTargetRange(TargetCharacter, CombatRadius)) {
		Combat->AttackCharacter(TargetCharacter);
	}
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
			3.f
		);
	}
}

void AEnemy::SetupActorForDefense()
{
	if (Combat) Combat->SetupDefendEvents();
}

void AEnemy::SetupActorForAttack()
{
	if (Combat) Combat->SetupAttackEvents();
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
			Character->GetAction()->ServerInitAttackCombat();
		}
	}
}

void AEnemy::OnSeeWizard(APawn* Pawn)
{
	if (Pawn->ActorHasTag(FName("WizardCharacter"))) {
		GetWorld()->GetTimerManager().ClearTimer(SensingTimer);
		TargetCharacter = Pawn;
		MoveToWizard(TargetCharacter);
	}
}
#pragma endregion

#pragma region Movement
void AEnemy::MoveEnemyToUnseenWizard()
{
	if (PawnSensing && PawnSensing->OnSeePawn.IsBound()) PawnSensing->OnSeePawn.RemoveDynamic(this, &AEnemy::OnSeeWizard);

	AWizardGameMode* WGameMode = Cast<AWizardGameMode>(GetWorld()->GetAuthGameMode());
	if (WGameMode) {
		AWizardCharacter* CharacterWithLowestAttribute = WGameMode->GetCharacterWithLowestAttribute(ChosenAttributeToChase);
		AWizardCharacter* ClosestCharacter = WGameMode->GetClosestCharacter(this);
		TargetCharacter = CharacterWithLowestAttribute == ClosestCharacter ? CharacterWithLowestAttribute : ClosestCharacter;
		if (TargetCharacter) {
			MoveToWizard(TargetCharacter);
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
		MoveRequest.SetAcceptanceRadius(175.f);
		EnemyController->MoveTo(MoveRequest);
	}
}

bool AEnemy::InTargetRange(AActor* Target, float Radius)
{
	if (Target == nullptr) return false;
	
	const float DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	return DistanceToTarget <= CombatRadius;
}

void AEnemy::StopEnemyMovement()
{
	EnemyController = EnemyController == nullptr ? Cast<AAIController>(Controller) : EnemyController;
	AWizardGameMode* WGameMode = Cast<AWizardGameMode>(GetWorld()->GetAuthGameMode());
	if (EnemyController) {
		EnemyController->StopMovement();
		if (TargetCharacter && WGameMode && !InTargetRange(TargetCharacter, CombatRadius)) WGameMode->IncrementEnemiesFinished();
	}
}
#pragma endregion
