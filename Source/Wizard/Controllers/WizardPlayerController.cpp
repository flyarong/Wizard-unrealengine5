// Copyright Epic Games, Inc. All Rights Reserved.

#include "WizardPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Wizard/Characters/WizardCharacter.h"
#include "Wizard/Components/Character/ActionComponent.h"
#include "Wizard/Components/Character/AttributeComponent.h"
#include "Wizard/Components/MiniMap/CharacterPOIComponent.h"
#include "Wizard/Pawns/GameplayCamera.h"
#include "Wizard/PlayerStates/WizardPlayerState.h"
#include "Wizard/HUD/WizardHUD.h"
#include "Wizard/GameModes/WizardGameMode.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"


AWizardPlayerController::AWizardPlayerController()
{
	bShowMouseCursor = false;
	DefaultMouseCursor = EMouseCursor::Default;

	CachedDestination = FVector::ZeroVector;
}

void AWizardPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	
	//Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	// Init Gameplay Camera pointer
	GameplayCamera = Cast<AGameplayCamera>(UGameplayStatics::GetActorOfClass(this, AGameplayCamera::StaticClass()));

	// Init HUD
	WizardHUD = Cast<AWizardHUD>(GetHUD());
	FInputModeUIOnly InputModeData;
	SetInputMode(InputModeData);
}

void AWizardPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AWizardPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWizardPlayerController, CachedDestination);
}

#pragma region Init
void AWizardPlayerController::InitOverlay()
{
	WizardHUD = WizardHUD == nullptr ? Cast<AWizardHUD>(GetHUD()) : WizardHUD;
	if (WizardHUD) {
		bWizardOverlayInitialized = WizardHUD->CreateWizardOverlay();
		if (bWizardOverlayInitialized && WizardCharacter && WizardCharacter->GetAttribute()) {
			SetHUDCurrentDistrict(EDistrict::ED_None);
			SetHUDEnergy(WizardCharacter->GetAttribute()->GetEnergy(), WizardCharacter->GetAttribute()->GetMaxEnergy());
			SetHUDXP(WizardCharacter->GetAttribute()->GetXP());
			SetHUDCharacterImage(WizardCharacter->GetPOI()->GetIconImage());
			SetHUDCharacterName(WizardCharacter->GetAttribute()->GetName());
			SetHUDCombat(WizardCharacter->GetAttribute()->GetCombat());
			SetHUDWisdom(WizardCharacter->GetAttribute()->GetWisdom());
			SetHUDIntelligence(WizardCharacter->GetAttribute()->GetIntelligence());
			SetHUDAgility(WizardCharacter->GetAttribute()->GetAgility());
		}
	}
}
#pragma endregion

void AWizardPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &AWizardPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &AWizardPlayerController::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &AWizardPlayerController::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &AWizardPlayerController::OnSetDestinationReleased);

		// Setup touch input events
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &AWizardPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &AWizardPlayerController::OnTouchTriggered);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &AWizardPlayerController::OnTouchReleased);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &AWizardPlayerController::OnTouchReleased);
	}

	// Camera Axis Mappings
	InputComponent->BindAxis(FName("MouseWheelAxis"), this, &AWizardPlayerController::MouseWheelAxis);
	InputComponent->BindAxis(FName("MoveForward"), this, &AWizardPlayerController::OnKeyMoveForward);
	InputComponent->BindAxis(FName("MoveRight"), this, &AWizardPlayerController::OnKeyMoveRight);
	InputComponent->BindAxis(FName("LookYaw"), this, &AWizardPlayerController::MouseRotateYaw);
}

#pragma region CharacterMovement
void AWizardPlayerController::OnInputStarted()
{
	ServerStopMovement();
	if (!HasAuthority()) StopMovement();
}

void AWizardPlayerController::ServerStopMovement_Implementation()
{
	StopMovement();
}

// Triggered every frame when the input is held down
void AWizardPlayerController::OnSetDestinationTriggered()
{	
	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	if (bIsTouch)
	{
		bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
	}
	else
	{
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	}

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}
}

void AWizardPlayerController::OnSetDestinationReleased()
{
	// We move there and spawn some particles
	ServerMoveToLocation(this, CachedDestination);
	if (!HasAuthority()) {
		// Move to location locally on client
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
	}
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		this, 
		FXCursor, 
		CachedDestination, 
		FRotator::ZeroRotator, 
		FVector(1.f, 1.f, 1.f), 
		true, 
		true, 
		ENCPoolMethod::None, 
		true
	);
}

void AWizardPlayerController::ServerMoveToLocation_Implementation(AWizardPlayerController* Controller, FVector Dest) {
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(Controller, Dest);
}

// Triggered every frame when the input is held down
void AWizardPlayerController::OnTouchTriggered()
{
	bIsTouch = true;
	OnSetDestinationTriggered();
}

void AWizardPlayerController::OnTouchReleased()
{
	bIsTouch = false;
	OnSetDestinationReleased();
}
#pragma endregion

#pragma region CameraMovement
void AWizardPlayerController::MouseWheelAxis(float Value)
{
	GameplayCamera = GameplayCamera == nullptr ? Cast<AGameplayCamera>(UGameplayStatics::GetActorOfClass(this, AGameplayCamera::StaticClass())) : GameplayCamera;
	if (GameplayCamera) {
		GameplayCamera->SetPositionWithMouseWheel(Value);
	}
}

void AWizardPlayerController::MouseRotateYaw(float Value)
{
	GameplayCamera = GameplayCamera == nullptr ? Cast<AGameplayCamera>(UGameplayStatics::GetActorOfClass(this, AGameplayCamera::StaticClass())) : GameplayCamera;
	if (GameplayCamera && IsInputKeyDown(EKeys::RightMouseButton)) {
		GameplayCamera->MouseRotate(Value);
	}
}

void AWizardPlayerController::OnKeyMoveForward(float Value)
{
	GameplayCamera = GameplayCamera == nullptr ? Cast<AGameplayCamera>(UGameplayStatics::GetActorOfClass(this, AGameplayCamera::StaticClass())) : GameplayCamera;
	if (GameplayCamera) GameplayCamera->KeyMoveForwardOrBackward(Value);
}

void AWizardPlayerController::OnKeyMoveRight(float Value)
{
	GameplayCamera = GameplayCamera == nullptr ? Cast<AGameplayCamera>(UGameplayStatics::GetActorOfClass(this, AGameplayCamera::StaticClass())) : GameplayCamera;
	if (GameplayCamera) GameplayCamera->KeyMoveLeftOrRight(Value);
}
#pragma endregion

#pragma region HUD/Player
void AWizardPlayerController::SetHUDCharacterImage(UTexture2D* CharacterImage)
{
	WizardHUD = WizardHUD == nullptr ? Cast<AWizardHUD>(GetHUD()) : WizardHUD;
	if (WizardHUD) {
		WizardHUD->SetCharacterImage(CharacterImage);
	}
}

void AWizardPlayerController::SetHUDCharacterName(FString CharacterName)
{
	WizardHUD = WizardHUD == nullptr ? Cast<AWizardHUD>(GetHUD()) : WizardHUD;
	if (WizardHUD) {
		WizardHUD->SetCharacterName(CharacterName);
	}
}

void AWizardPlayerController::SetHUDCurrentDistrict(EDistrict District)
{
	WizardHUD = WizardHUD == nullptr ? Cast<AWizardHUD>(GetHUD()) : WizardHUD;
	if (WizardHUD && District != EDistrict::ED_None) {
		WizardHUD->SetCurrentDistrict(District);
	}
}

void AWizardPlayerController::SetHUDEnergy(float Energy, float MaxEnergy)
{
	WizardHUD = WizardHUD == nullptr ? Cast<AWizardHUD>(GetHUD()) : WizardHUD;
	if (WizardHUD) {
		WizardHUD->SetEnergy(Energy, MaxEnergy);
	}
}

void AWizardPlayerController::SetHUDXP(int32 NewXP)
{
	WizardHUD = WizardHUD == nullptr ? Cast<AWizardHUD>(GetHUD()) : WizardHUD;
	if (WizardHUD) {
		WizardHUD->SetXP(NewXP);
	}
}

void AWizardPlayerController::SetHUDCombat(int32 NewCombat)
{
	WizardHUD = WizardHUD == nullptr ? Cast<AWizardHUD>(GetHUD()) : WizardHUD;
	if (WizardHUD) {
		WizardHUD->SetCombat(NewCombat);
	}
}

void AWizardPlayerController::SetHUDWisdom(int32 NewWisdom)
{
	WizardHUD = WizardHUD == nullptr ? Cast<AWizardHUD>(GetHUD()) : WizardHUD;
	if (WizardHUD) {
		WizardHUD->SetWisdom(NewWisdom);
	}
}

void AWizardPlayerController::SetHUDIntelligence(int32 NewIntelligence)
{
	WizardHUD = WizardHUD == nullptr ? Cast<AWizardHUD>(GetHUD()) : WizardHUD;
	if (WizardHUD) {
		WizardHUD->SetIntelligence(NewIntelligence);
	}
}

void AWizardPlayerController::SetHUDAgility(int32 NewAgility)
{
	WizardHUD = WizardHUD == nullptr ? Cast<AWizardHUD>(GetHUD()) : WizardHUD;
	if (WizardHUD) {
		WizardHUD->SetAgility(NewAgility);
	}
}

#pragma endregion

#pragma region HUD/Store/Catalog
void AWizardPlayerController::SetHUDStoreCatalog(AStore* Store)
{
	WizardHUD = WizardHUD == nullptr ? Cast<AWizardHUD>(GetHUD()) : WizardHUD;
	if (WizardHUD) {
		WizardHUD->SetStoreCatalog(Store);
	}
}

void AWizardPlayerController::AddHUDCharacterItem(int32 ItemIndex)
{
	WizardHUD = WizardHUD == nullptr ? Cast<AWizardHUD>(GetHUD()) : WizardHUD;
	if (WizardHUD) {
		WizardHUD->AddCharacterItem(ItemIndex);
	}
}
#pragma endregion

#pragma region HUD/MiniMap
void AWizardPlayerController::SetHUDPOIOnMiniMap(AActor* POIOwner)
{
	WizardHUD = WizardHUD == nullptr ? Cast<AWizardHUD>(GetHUD()) : WizardHUD;
	if (POIOwner && WizardHUD) {
		WizardHUD->SetPOIOnMiniMap(POIOwner);
	}
}
#pragma endregion