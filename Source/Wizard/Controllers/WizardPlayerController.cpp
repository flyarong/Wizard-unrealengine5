// Copyright Epic Games, Inc. All Rights Reserved.

#include "WizardPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Wizard/Actors/WizardActor.h"
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
	EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (EnhancedInputSubsystem)
	{
		EnhancedInputSubsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	// Init HUD
	WizardHUD = Cast<AWizardHUD>(GetHUD());
}

void AWizardPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AWizardPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWizardPlayerController, CachedDestination);
	DOREPLIFETIME(AWizardPlayerController, bCanCastSpell);
}

#pragma region Init
void AWizardPlayerController::SetupCamera()
{
	UWorld* World = GetWorld();
	if (World && WizardCharacter) {
		GameplayCamera = World->SpawnActor<AGameplayCamera>(WizardCharacter->GetActorLocation(), WizardCharacter->GetActorRotation());
		if (GameplayCamera && IsLocalController()) {
			GameplayCamera->SetWizard(WizardCharacter);
			SetViewTargetWithBlend(GameplayCamera);
			SetCameraFocusOnWizard();
		}
	}
}

void AWizardPlayerController::SetWizardCharacter(AWizardCharacter* WCharacter)
{
	WizardCharacter = WCharacter;
}

void AWizardPlayerController::InitOverlay()
{
	WizardHUD = WizardHUD == nullptr ? Cast<AWizardHUD>(GetHUD()) : WizardHUD;
	if (WizardHUD) {
		bWizardOverlayInitialized = WizardHUD->CreateWizardOverlay();
		if (bWizardOverlayInitialized && WizardCharacter && WizardCharacter->GetAttribute()) {
			SetHUDCurrentDistrict(EDistrict::ED_None);
			SetHUDHealth(WizardCharacter->GetAttribute()->GetHealth(), WizardCharacter->GetAttribute()->GetMaxHealth());
			SetHUDPower(WizardCharacter->GetAttribute()->GetPower(), WizardCharacter->GetAttribute()->GetMaxPower());
			SetHUDXP(WizardCharacter->GetAttribute()->GetXP());
			SetHUDCharacterImage(WizardCharacter->GetPOI()->GetIconImage());
			SetHUDCharacterName(WizardCharacter->GetAttribute()->GetName());
			SetHUDOffense(WizardCharacter->GetAttribute()->GetOffense());
			SetHUDDefense(WizardCharacter->GetAttribute()->GetDefense());
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

		// Setup Camera input events
		EnhancedInputComponent->BindAction(CameraMoveAction, ETriggerEvent::Triggered, this, &AWizardPlayerController::OnKeyMove);
		EnhancedInputComponent->BindAction(CameraZoomAction, ETriggerEvent::Triggered, this, &AWizardPlayerController::OnMouseWheelAxis);
		EnhancedInputComponent->BindAction(CameraRotateAction, ETriggerEvent::Triggered, this, &AWizardPlayerController::OnMouseRotateYaw);

		// Setup Combat input events
		EnhancedInputComponent->BindAction(CombatAction, ETriggerEvent::Started, this, &AWizardPlayerController::OnCombatKeyStarted);
	}
}

void AWizardPlayerController::SetInputContext(EInputContext ContextType)
{
	FModifyContextOptions Options;
	Options.bForceImmediately = false;
	Options.bIgnoreAllPressedKeysUntilRelease = true;

	switch (ContextType)
	{
	case EInputContext::EIC_Default:
		if (GameplayCamera) GameplayCamera->SetEnableCameraMovementWithMouse(false);
		if (EnhancedInputSubsystem->HasMappingContext(CombatMappingContext)) {
			EnhancedInputSubsystem->RemoveMappingContext(CombatMappingContext, Options);
		}
		EnhancedInputSubsystem->AddMappingContext(DefaultMappingContext, 0, Options);
		SetCameraPositionToDefault();
		break;
	case EInputContext::EIC_Combat:
		if (GameplayCamera) GameplayCamera->SetEnableCameraMovementWithMouse(false);
		if (EnhancedInputSubsystem->HasMappingContext(DefaultMappingContext)) {
			EnhancedInputSubsystem->RemoveMappingContext(DefaultMappingContext, Options);
		}
		EnhancedInputSubsystem->AddMappingContext(CombatMappingContext, 0, Options);
		SetCameraPositionToCombat();
		break;
	case EInputContext::EIC_None:
		if (EnhancedInputSubsystem->HasMappingContext(DefaultMappingContext)) {
			EnhancedInputSubsystem->RemoveMappingContext(DefaultMappingContext, Options);
		}
		if (EnhancedInputSubsystem->HasMappingContext(CombatMappingContext)) {
			EnhancedInputSubsystem->RemoveMappingContext(CombatMappingContext, Options);
		}
		break;
	default:
		break;
	}
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
	bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);

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
#pragma endregion

#pragma region CameraMovement
void AWizardPlayerController::SetCameraFocusOnWizard()
{
	if (GameplayCamera) {
		GameplayCamera->SetCameraFocusOnWizard();
	}
}

void AWizardPlayerController::SetCameraPositionToDefault()
{
	if (GameplayCamera) {
		GameplayCamera->SetPositionToDefault();
	}
}

void AWizardPlayerController::SetCameraPositionToCombat()
{
	if (GameplayCamera) {
		GameplayCamera->SetPositionToCombat();
	}
}

void AWizardPlayerController::OnMouseWheelAxis(const FInputActionValue& ActionValue)
{
	if (GameplayCamera) {
		GameplayCamera->SetPositionWithMouseWheel(ActionValue.Get<float>());
	}
}

void AWizardPlayerController::OnMouseRotateYaw(const FInputActionValue& ActionValue)
{
	if (GameplayCamera) {
		GameplayCamera->MouseRotate(ActionValue.Get<float>());
	}
}

void AWizardPlayerController::OnKeyMove(const FInputActionValue& ActionValue)
{
	if (GameplayCamera) {
		GameplayCamera->KeyMove(ActionValue.Get<FVector>());
	}
}
#pragma endregion

#pragma region Combat
void AWizardPlayerController::OnCombatKeyStarted(const FInputActionValue& ActionValue)
{
	if (IsLocalController() && WizardCharacter && WizardCharacter->GetAction()) {
		int32 KeyIndex = UKismetMathLibrary::Round(ActionValue.Get<float>());
		WizardCharacter->GetAction()->ServerValidateCombatInput(KeyIndex);
	}
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

void AWizardPlayerController::SetHUDHealth(float Health, float MaxHealth)
{
	WizardHUD = WizardHUD == nullptr ? Cast<AWizardHUD>(GetHUD()) : WizardHUD;
	if (WizardHUD) {
		WizardHUD->SetHealth(Health, MaxHealth);
	}
}

void AWizardPlayerController::SetHUDPower(float Power, float MaxPower)
{
	WizardHUD = WizardHUD == nullptr ? Cast<AWizardHUD>(GetHUD()) : WizardHUD;
	if (WizardHUD) {
		WizardHUD->SetPower(Power, MaxPower);
	}
}

void AWizardPlayerController::SetHUDXP(int32 NewXP)
{
	WizardHUD = WizardHUD == nullptr ? Cast<AWizardHUD>(GetHUD()) : WizardHUD;
	if (WizardHUD) {
		WizardHUD->SetXP(NewXP);
	}
}

void AWizardPlayerController::SetHUDOffense(int32 NewOffense)
{
	WizardHUD = WizardHUD == nullptr ? Cast<AWizardHUD>(GetHUD()) : WizardHUD;
	if (WizardHUD) {
		WizardHUD->SetOffense(NewOffense);
	}
}

void AWizardPlayerController::SetHUDDefense(int32 NewDefense)
{
	WizardHUD = WizardHUD == nullptr ? Cast<AWizardHUD>(GetHUD()) : WizardHUD;
	if (WizardHUD) {
		WizardHUD->SetDefense(NewDefense);
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

void AWizardPlayerController::ServerDestroyPOI_Implementation(AActor* POIOwner)
{
	WizardGameMode = WizardGameMode == nullptr ? Cast<AWizardGameMode>(UGameplayStatics::GetGameMode(this)) : WizardGameMode;
	if (WizardGameMode && POIOwner) {
		WizardGameMode->RemoveMiniMapActor(POIOwner);
	}
}
#pragma endregion

#pragma region HUD/Messages
void AWizardPlayerController::AddHUDLocalMessage(const FString& Message, EAttribute AttributeType)
{
	WizardHUD = WizardHUD == nullptr ? Cast<AWizardHUD>(GetHUD()) : WizardHUD;
	if (WizardHUD) {
		WizardHUD->AddLocalMessage(Message, AttributeType);
	}
}

void AWizardPlayerController::ClientAddHUDVictoryPublicMessage_Implementation(AWizardCharacter* WCharacter, AWizardActor* DefeatedActor)
{
	WizardHUD = WizardHUD == nullptr ? Cast<AWizardHUD>(GetHUD()) : WizardHUD;
	if (WizardHUD) {
		WizardHUD->AddPublicMessage(WCharacter, EAction::EA_Combat, DefeatedActor);
	}
}

void AWizardPlayerController::ServerSendChatMessage_Implementation(const FString& Sender, const FText& Message)
{
	WizardGameMode = WizardGameMode == nullptr ? Cast<AWizardGameMode>(GetWorld()->GetAuthGameMode()) : WizardGameMode;
	if (WizardGameMode) {
		FString PlayerName = Sender;
		WizardGameMode->BroadcastChatMessage(
			FText::FromString(PlayerName
				.Append(FString(TEXT(": ")))
				.Append(Message.ToString()))
		);
	}
}

void AWizardPlayerController::ClientAddHUDChatMessage_Implementation(const FText& Message)
{
	WizardHUD = WizardHUD == nullptr ? Cast<AWizardHUD>(GetHUD()) : WizardHUD;
	if (WizardHUD) {
		WizardHUD->AddChatMessage(Message);
	}
}
#pragma endregion

#pragma region HUD/Combat
void AWizardPlayerController::CreateHUDSpellMap(TArray<FKey>& SpellInputs, TArray<int32>& SpellIndexes)
{
	TMap<FKey, int32> SpellMap;
	for (int32 i = 0; i < SpellInputs.Num(); i++) {
		SpellMap.Add(SpellInputs[i], SpellIndexes[i]);
	}

	WizardHUD = WizardHUD == nullptr ? Cast<AWizardHUD>(GetHUD()) : WizardHUD;
	if (WizardHUD) {
		WizardHUD->CreateSpellMap(SpellMap);
	}
}

void AWizardPlayerController::AddHUDSpellMap()
{
	WizardHUD = WizardHUD == nullptr ? Cast<AWizardHUD>(GetHUD()) : WizardHUD;
	if (WizardHUD) {
		WizardHUD->AddSpellMap();
	}
}

void AWizardPlayerController::RemoveSpellMapFromHUD()
{
	WizardHUD = WizardHUD == nullptr ? Cast<AWizardHUD>(GetHUD()) : WizardHUD;
	if (WizardHUD) {
		WizardHUD->RemoveSpellMap();
	}
}

void AWizardPlayerController::AddHUDCombatMenu()
{
	WizardHUD = WizardHUD == nullptr ? Cast<AWizardHUD>(GetHUD()) : WizardHUD;
	if (WizardHUD) {
		WizardHUD->HideCurrentDistrict();
		WizardHUD->HideLeftPanel();
		WizardHUD->CreateCombatScore();
		WizardHUD->AddCombatMenu();
	}
}

void AWizardPlayerController::ResetHUD()
{
	WizardHUD = WizardHUD == nullptr ? Cast<AWizardHUD>(GetHUD()) : WizardHUD;
	if (WizardHUD) {
		SetShowMouseCursor(true);
		WizardHUD->ClearCenterBox();
		WizardHUD->ClearBottomBox();
		WizardHUD->RemoveSpellMap();
		WizardHUD->ShowLeftPanel();
	}
}

void AWizardPlayerController::AddHUDCurrentSpellStep(int32 CurrentStepIndex)
{
	WizardHUD = WizardHUD == nullptr ? Cast<AWizardHUD>(GetHUD()) : WizardHUD;
	if (WizardHUD) {
		WizardHUD->AddCurrentSpellStep(CurrentStepIndex);
	}
}

void AWizardPlayerController::RemoveHUDPreviouseSpellStep()
{
	WizardHUD = WizardHUD == nullptr ? Cast<AWizardHUD>(GetHUD()) : WizardHUD;
	if (WizardHUD) {
		WizardHUD->ClearCenterBox();
	}
}

void AWizardPlayerController::AddHUDSpellStepResult(bool bWasSuccessful)
{
	WizardHUD = WizardHUD == nullptr ? Cast<AWizardHUD>(GetHUD()) : WizardHUD;
	if (WizardHUD) {
		WizardHUD->AddSpellStepResult(bWasSuccessful);
	}
}

void AWizardPlayerController::AddHUDCombatScore(int32 Score)
{
	WizardHUD = WizardHUD == nullptr ? Cast<AWizardHUD>(GetHUD()) : WizardHUD;
	if (WizardHUD) {
		WizardHUD->AddCombatScore(Score);
	}
}
#pragma endregion