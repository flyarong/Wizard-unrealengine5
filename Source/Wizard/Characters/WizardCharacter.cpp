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
#include "Kismet/GameplayStatics.h"
#include "Wizard/HUD/LobbyHUD.h"
#include "Wizard/HUD/WizardWidgetClasses/OverheadWidget.h"
#include "Wizard/Components/Character/ActionComponent.h"

AWizardCharacter::AWizardCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create Action Component
	Action = CreateDefaultSubobject<UActionComponent>(TEXT("Action"));
	Action->SetIsReplicated(true);

	// Create Overhead widget
	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(RootComponent);

	// Create Magic Staff
	MagicStaff = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MagicStaff"));
	MagicStaff->SetupAttachment(GetMesh(), FName("WeaponSocket"));

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

void AWizardCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (Action) {
		Action->Character = this;
	}
}

void AWizardCharacter::InitGameplayCharacter(FString PlayerName, FName RowName)
{
	const FString Path{ CHARACTER_DATA_TABLE_PATH };
	UDataTable* CharacterTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *Path));
	if (CharacterTableObject && !RowName.IsNone()) {
		FCharacterDataTable* SelectedCharacter = CharacterTableObject->FindRow<FCharacterDataTable>(RowName, TEXT(""));

		// Setup character properties when story starts
		if (Action && GetMesh() && MagicStaff && SelectedCharacter) {
			GetMesh()->SetSkeletalMesh(SelectedCharacter->CharacterMesh);
			MagicStaff->SetStaticMesh(SelectedCharacter->MagicStaff);

			Name = SelectedCharacter->CharacterName;
			Health = SelectedCharacter->Health;
			Power = SelectedCharacter->Power;
			Action->SetWisdom(SelectedCharacter->Wisdom);
			Action->SetIntelligence(SelectedCharacter->Intelligence);
			Action->SetCombat(SelectedCharacter->Combat);
			Action->SetAgility(SelectedCharacter->Agility);
		}
	}

	if (!PlayerName.IsEmpty() && OverheadWidget) {
		UOverheadWidget* Overhead = Cast<UOverheadWidget>(OverheadWidget->GetWidget());
		if (Overhead) {
			Overhead->SetDisplayText(PlayerName);
		}
	}
}


