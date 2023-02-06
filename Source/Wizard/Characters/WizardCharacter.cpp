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
#include "Wizard/Components/MiniMap/CharacterPOIComponent.h"
#include "Wizard/WizardTypes/BoostTypes.h"

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

	DOREPLIFETIME(AWizardCharacter, ItemIndexes);
	DOREPLIFETIME(AWizardCharacter, LatestItem);
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
			Attribute->SetHealth(SelectedCharacter->Health);
			Attribute->SetPower(SelectedCharacter->Power);
			Attribute->SetWisdom(SelectedCharacter->Wisdom);
			Attribute->SetIntelligence(SelectedCharacter->Intelligence);
			Attribute->SetCombat(SelectedCharacter->Combat);
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

	// Setup overlay on HUD and enable gameplay input mode
	PlayerController = Cast<AWizardPlayerController>(Controller);
	if (PlayerController) {
		PlayerController->SetWizardCharacter(this);
		// Init WizardOverlay
		PlayerController->InitOverlay();

		// Setup Point of Interest on Minimap
		POI->SetupPOI(this);

		FInputModeGameAndUI InputModeData;
		PlayerController->SetInputMode(InputModeData);
		PlayerController->SetShowMouseCursor(true);
	}
}

#pragma region Items
void AWizardCharacter::AddNewItem(int32 ItemIndex, FItemDataTable ItemRow)
{
	ItemIndexes.Add(ItemIndex);
	LatestItem = ItemRow;
	Items.Add(ItemIndexes.Last(), LatestItem);

	Attribute->SpendXP(LatestItem.Cost);
	AddHUDItem(ItemIndexes.Last());
}

void AWizardCharacter::OnRep_ItemIndexes()
{
	AddHUDItem(ItemIndexes.Last());
}

void AWizardCharacter::AddHUDItem(int32 ItemIndex)
{
	PlayerController = PlayerController == nullptr ? Cast<AWizardPlayerController>(Controller) : PlayerController;
	if (PlayerController) {
		PlayerController->AddHUDCharacterItem(ItemIndex);
	}
}

void AWizardCharacter::ServerUseItem_Implementation(int32 ItemIndex)
{
	if (!ItemIndexes.Contains(ItemIndex) || !Items.Contains(ItemIndex)) {
		return;
	}

	switch (Items[ItemIndex].BoostType)
	{
	case EBoost::EB_Health:
		break;
	case EBoost::EB_Power:
		break;
	case EBoost::EB_Energy:
		Attribute->AddEnergy(Items[ItemIndex].BoostAmount);
		break;
	case EBoost::EB_Wisdom:
		break;
	case EBoost::EB_Intelligence:
		break;
	case EBoost::EB_Combat:
		break;
	case EBoost::EB_Agility:
		break;
	default:
		break;
	}
}
#pragma endregion