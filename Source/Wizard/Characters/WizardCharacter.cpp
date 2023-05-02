// Copyright Epic Games, Inc. All Rights Reserved.

#include "WizardCharacter.h"
#include "Wizard/Controllers/WizardPlayerController.h"
#include "Wizard/GameModes/WizardGameMode.h"
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

void AWizardCharacter::BeginPlay()
{
	Super::BeginPlay();

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
	DOREPLIFETIME(AWizardCharacter, Equipments);
	DOREPLIFETIME(AWizardCharacter, Outfit);
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
			Tags.Add(FName("WizardCharacter"));
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
		if (HasAuthority()) PlayerController->OnMatchStateSet(MatchState::InProgress);
		
		if (IsLocallyControlled()) {
			// Setup Controller&Gameplay Camera
			PlayerController->SetupCamera();

			// Setup WizardOverlay
			PlayerController->SetupOverlay();
			PlayerController->SetShowMouseCursor(true);

			// Setup Point of Interest on Minimap
			POI->ServerSetupPOI(this);
		}
	}
}

void AWizardCharacter::LeaveGame()
{
	ServerLeaveGame();
	if (IsLocallyControlled()) OnLeftGameDelegate.Broadcast();
}

void AWizardCharacter::ServerLeaveGame_Implementation()
{
	AWizardGameMode* WGameMode = Cast<AWizardGameMode>(GetWorld()->GetAuthGameMode());
	if (WGameMode && PlayerController) {
		WGameMode->PlayerLeavingGame(PlayerController);
	}
}

#pragma region Items
void AWizardCharacter::AddNewItem(const FItemDataTable& ItemRow)
{
	if (ItemRow.bIsPersistent) {
		Equipments.Add(ItemRow);
		UpdateEquipments();
	}
	else {
		Items.Add(ItemRow);
		UpdateInventory();
	}
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

void AWizardCharacter::OnRep_Equipments()
{
	UpdateEquipments();
}

void AWizardCharacter::UpdateEquipments()
{
	PlayerController = PlayerController == nullptr ? Cast<AWizardPlayerController>(Controller) : PlayerController;
	if (PlayerController) {
		PlayerController->UpdateHUDCharacterEquipments(Equipments);
	}
}

void AWizardCharacter::OnRep_Outfit()
{
	UpdateOutfit();
}

void AWizardCharacter::UpdateOutfit()
{
	PlayerController = PlayerController == nullptr ? Cast<AWizardPlayerController>(Controller) : PlayerController;
	if (PlayerController) {
		PlayerController->UpdateHUDCharacterOutfit(Outfit);
	}
}

void AWizardCharacter::ServerUseItem_Implementation(const FItemDataTable& Item)
{
	if (!Items.Contains(Item)) return;

	BoostAttribute(Item);

	Items.RemoveSingle(Item);
	UpdateInventory();
	ServerInterruptMovement();
	MulticastPlayInteractMontage(FName("Use"));
}

void AWizardCharacter::BoostAttribute(const FItemDataTable& Item)
{
	switch (Item.BoostType)
	{
	case EBoost::EB_Health:
		if (Attribute->GetHealth() >= Attribute->GetMaxHealth()) return;
		Attribute->AddHealth(Item.BoostAmount);
		break;
	case EBoost::EB_Power:
		if (Attribute->GetPower() >= Attribute->GetMaxPower()) return;
		Attribute->AddPower(Item.BoostAmount);
		break;
	case EBoost::EB_Defense:
		Attribute->IncreaseDefense(Item.BoostAmount); // TODO check later for all if persistent; if not, one-time boost applied
		break;
	case EBoost::EB_Wisdom:
		Attribute->IncreaseWisdom(Item.BoostAmount);
		break;
	case EBoost::EB_Intelligence:
		Attribute->IncreaseIntelligence(Item.BoostAmount);
		break;
	case EBoost::EB_Offense:
		Attribute->IncreaseOffense(Item.BoostAmount);
		break;
	case EBoost::EB_Agility:
		Attribute->IncreaseAgility(Item.BoostAmount);
		break;
	default:
		break;
	}
}

void AWizardCharacter::DecreaseAttribute(const FItemDataTable& Item)
{
	switch (Item.BoostType)
	{
	case EBoost::EB_Defense:
		Attribute->DecreaseDefense(Item.BoostAmount);
		break;
	case EBoost::EB_Wisdom:
		Attribute->DecreaseWisdom(Item.BoostAmount);
		break;
	case EBoost::EB_Intelligence:
		Attribute->DecreaseIntelligence(Item.BoostAmount);
		break;
	case EBoost::EB_Offense:
		Attribute->DecreaseOffense(Item.BoostAmount);
		break;
	case EBoost::EB_Agility:
		Attribute->DecreaseAgility(Item.BoostAmount);
		break;
	default:
		break;
	}
}

void AWizardCharacter::ServerEquipOutfit_Implementation(const FItemDataTable& Equipment)
{
	if (!Equipments.Contains(Equipment) || HasOutfitTypeOn(Equipment.OutfitType)) return;

	BoostAttribute(Equipment);

	Equipments.RemoveSingle(Equipment);
	UpdateEquipments();
	Outfit.Add(Equipment);
	UpdateOutfit();
}

bool AWizardCharacter::HasOutfitTypeOn(EOutfit OutfitType)
{
	for (auto& OutfitElement : Outfit) {
		if (OutfitElement.OutfitType == OutfitType) return true;
	}

	return false;
}

void AWizardCharacter::ServerUnEquipOutfit_Implementation(const FItemDataTable& Equipment)
{
	if (!Outfit.Contains(Equipment)) return;

	DecreaseAttribute(Equipment);

	Outfit.RemoveSingle(Equipment);
	UpdateOutfit();
	Equipments.Add(Equipment);
	UpdateEquipments();
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

void AWizardCharacter::OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (HasAuthority() && Combat && NotifyName.IsEqual(FName("EndCombat"))) {
		Combat->EndCombat();
	}
	else if (HasAuthority() && Combat && Action && Action->GetIsInCombat() && NotifyName.IsEqual(FName("EndPickup"))) {
		Combat->EndCombat();
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

void AWizardCharacter::ServerInterruptMovement_Implementation()
{
	MulticastInterruptMovement();
}

void AWizardCharacter::MulticastInterruptMovement_Implementation()
{
	PlayerController = PlayerController == nullptr ? Cast<AWizardPlayerController>(Controller) : PlayerController;
	if (PlayerController) {
		PlayerController->StopMovement();
	}
}
