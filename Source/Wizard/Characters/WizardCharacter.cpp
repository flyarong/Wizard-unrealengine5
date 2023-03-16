// Copyright Epic Games, Inc. All Rights Reserved.

#include "WizardCharacter.h"
#include "Wizard/Controllers/WizardPlayerController.h"
#include "Wizard/Wizard.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Wizard/HUD/LobbyHUD.h"
#include "Wizard/HUD/WizardWidgetClasses/OverheadWidget.h"
#include "Wizard/Components/Character/ActionComponent.h"
#include "Wizard/Components/Character/AttributeComponent.h"
#include "Wizard/Components/Character/CombatComponent.h"
#include "Wizard/Components/MiniMap/CharacterPOIComponent.h"
#include "Wizard/WizardTypes/BoostTypes.h"
#include "Wizard/Characters/WizardAnimInstance.h"
#include "Sound/SoundCue.h"

AWizardCharacter::AWizardCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create Attribute Component
	Attribute = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attribute"));
	Attribute->SetIsReplicated(true);

	// Create Action Component
	Action = CreateDefaultSubobject<UActionComponent>(TEXT("Action"));
	Action->SetIsReplicated(true);

	// Create Combat Component
	Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat"));
	Combat->SetIsReplicated(true);

	// Create Combat Mesh and set it as Root
	CombatMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CombatMesh"));
	CombatMesh->SetupAttachment(RootComponent);
	CombatMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	CombatMesh->SetRelativeLocation(FVector(-20.f, 115.f, 0.f));
	CombatMesh->SetRelativeRotation(FRotator(180.f, 40.f, 90.f));
	CombatMesh->SetRelativeScale3D(FVector(.5f, .5f, .5f));
	Combat->SetIsReplicated(true);

	// Create Point of Interest Component
	POI = CreateDefaultSubobject<UCharacterPOIComponent>(TEXT("PointOfInterest"));
	POI->SetIsReplicated(true);

	// Create Overhead widget
	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(RootComponent);

	// Create Magic Staff
	MagicStaff = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MagicStaff"));
	MagicStaff->SetupAttachment(GetMesh(), FName("WeaponSocket"));
	MagicStaff->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AWizardCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void AWizardCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AWizardCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWizardCharacter, Items);
}

void AWizardCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (Action) {
		Action->Character = this;
	}
	if (Attribute) {
		Attribute->Character = this;
	}
	if (Combat) {
		Combat->Character = this;
		CombatMesh->SetStaticMesh(CombatSM);
		CombatMesh->SetMaterial(0, CombatMaterial);
		Combat->SpellBar = CombatMesh->CreateDynamicMaterialInstance(0, CombatMesh->GetMaterial(0));
	}
}

UTexture2D* AWizardCharacter::GetIcon()
{
	return POI->GetIconImage();
}

void AWizardCharacter::InitGameplayCharacter(FString PlayerName, FName RowName)
{
	const FString Path{ CHARACTER_DATA_TABLE_PATH };
	UDataTable* CharacterTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *Path));
	if (CharacterTableObject && !RowName.IsNone()) {
		FCharacterDataTable* SelectedCharacter = CharacterTableObject->FindRow<FCharacterDataTable>(RowName, TEXT(""));

		// Setup character properties when story starts
		if (Attribute && GetMesh() && MagicStaff && SelectedCharacter) {
			GetMesh()->SetSkeletalMesh(SelectedCharacter->CharacterMesh);
			MagicStaff->SetStaticMesh(SelectedCharacter->MagicStaff);
			
			Attribute->SetName(SelectedCharacter->CharacterName);
			Attribute->SetDefense(SelectedCharacter->Defense);
			Attribute->SetWisdom(SelectedCharacter->Wisdom);
			Attribute->SetIntelligence(SelectedCharacter->Intelligence);
			Attribute->SetOffense(SelectedCharacter->Offense);
			Attribute->SetAgility(SelectedCharacter->Agility);

			POI->bIsStatic = true;
			POI->IconImage = SelectedCharacter->MiniMapIcon;
		}
	}

	if (!PlayerName.IsEmpty() && OverheadWidget) {
		UOverheadWidget* Overhead = Cast<UOverheadWidget>(OverheadWidget->GetWidget());
		if (Overhead) {
			Overhead->SetDisplayText(PlayerName);
		}
	}

	PlayerController = Cast<AWizardPlayerController>(Controller);
	if (PlayerController) {
		// Setup Controller&Gameplay Camera
		PlayerController->SetWizardCharacter(this);
		PlayerController->SetupCamera();

		// Init WizardOverlay
		PlayerController->InitOverlay();
		PlayerController->SetShowMouseCursor(true);

		// Setup Point of Interest on Minimap
		POI->SetupPOI(this);
	}
}

#pragma region Items
void AWizardCharacter::AddNewItem(const FItemDataTable& ItemRow)
{
	Items.Add(ItemRow);
	UpdateInventory();
}

void AWizardCharacter::OnRep_Items()
{
	UpdateInventory();
}

void AWizardCharacter::UpdateInventory()
{
	PlayerController = PlayerController == nullptr ? Cast<AWizardPlayerController>(Controller) : PlayerController;
	if (PlayerController) {
		PlayerController->UpdateHUDCharacterInventory(Items);
	}
}

void AWizardCharacter::ServerUseItem_Implementation(const FItemDataTable& Item)
{
	if (!Items.Contains(Item)) return;

	switch (Item.BoostType)
	{
	case EBoost::EB_Health:
		break;
	case EBoost::EB_Defense:
		break;
	case EBoost::EB_Power:
		if (Attribute->GetPower() >= Attribute->GetMaxPower()) return;
		Attribute->AddPower(Item.BoostAmount);
		break;
	case EBoost::EB_Wisdom:
		break;
	case EBoost::EB_Intelligence:
		break;
	case EBoost::EB_Offense:
		break;
	case EBoost::EB_Agility:
		break;
	default:
		break;
	}

	Items.RemoveSingle(Item);
	UpdateInventory();
	InterruptMovement();
	MulticastPlayInteractMontage(FName("Use"));
}
#pragma endregion

void AWizardCharacter::MulticastPlayInteractMontage_Implementation(FName Section)
{
	PlayMontage(InteractMontage, Section);
}

void AWizardCharacter::PlayMontage(UAnimMontage* Montage, FName Section)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance) {
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(Section);
	}
}

void AWizardCharacter::PlaySound(USoundCue* Sound)
{
	if (Sound) {
		UGameplayStatics::PlaySoundAtLocation(
			this,
			Sound,
			GetActorLocation()
		);
	}
}

void AWizardCharacter::InterruptMovement()
{
	PlayerController = PlayerController == nullptr ? Cast<AWizardPlayerController>(Controller) : PlayerController;
	if (PlayerController) {
		PlayerController->InterruptCharacterMovement();
	}
}
