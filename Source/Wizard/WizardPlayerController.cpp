// Copyright Epic Games, Inc. All Rights Reserved.

#include "WizardPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "WizardCharacter.h"
#include "Wizard/Pawns/GameplayCamera.h"
#include "Wizard/PlayerStates/WizardPlayerState.h"
#include "Wizard/WizardGameMode.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"


AWizardPlayerController::AWizardPlayerController()
{
	bShowMouseCursor = true;
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
}

void AWizardPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (HasAuthority() && !bCharacterInitialized) {
		WizardGameMode = Cast<AWizardGameMode>(UGameplayStatics::GetGameMode(this));
		WizardPlayerState = GetPlayerState<AWizardPlayerState>();
		if (WizardGameMode && WizardPlayerState) {
			FName WizardName = WizardGameMode->GetPlayerCharacter(WizardPlayerState->GetPlayerName());
			InitCharacter(WizardName);
		}
	}
}

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
}

void AWizardPlayerController::InitCharacter(FName CharacterName)
{
	WizardPlayerState = WizardPlayerState == nullptr ? GetPlayerState<AWizardPlayerState>() : WizardPlayerState;
	if (WizardPlayerState && !CharacterName.IsNone()) {

		// Set timer to wait for client Pawn to be valid
		FTimerHandle CharacterInitTimer;
		FTimerDelegate CharacterInitDelegate;
		CharacterInitDelegate.BindUFunction(WizardPlayerState, FName("SetSelectedCharacter"), CharacterName);

		GetWorldTimerManager().SetTimer(
			CharacterInitTimer,
			CharacterInitDelegate,
			3.f,
			false
		);

		bCharacterInitialized = true;
	}
}

#pragma region CharacterMovement
void AWizardPlayerController::OnInputStarted()
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
		GameplayCamera->MouseWheelAxis(Value);
	}
}

void AWizardPlayerController::OnKeyMoveForward(float Value)
{
	GameplayCamera = GameplayCamera == nullptr ? Cast<AGameplayCamera>(UGameplayStatics::GetActorOfClass(this, AGameplayCamera::StaticClass())) : GameplayCamera;
	if (GameplayCamera) {
		if (Value > 0) {
			FVector DeltaLocation = FVector(0.f, -GameplayCamera->GetCameraMovementSpeed(), 0.f);
			if (GameplayCamera->GetCameraBoom()) GameplayCamera->GetCameraBoom()->AddRelativeLocation(DeltaLocation);
		}
		else if (Value < 0) {
			FVector DeltaLocation = FVector(0.f, GameplayCamera->GetCameraMovementSpeed(), 0.f);
			if (GameplayCamera->GetCameraBoom()) GameplayCamera->GetCameraBoom()->AddRelativeLocation(DeltaLocation);
		}
	}
}

void AWizardPlayerController::OnKeyMoveRight(float Value)
{
	GameplayCamera = GameplayCamera == nullptr ? Cast<AGameplayCamera>(UGameplayStatics::GetActorOfClass(this, AGameplayCamera::StaticClass())) : GameplayCamera;
	if (GameplayCamera) {
		if (Value > 0) {
			FVector DeltaLocation = FVector(GameplayCamera->GetCameraMovementSpeed(), 0.f, 0.f);
			if (GameplayCamera->GetCameraBoom()) GameplayCamera->GetCameraBoom()->AddRelativeLocation(DeltaLocation);
		}
		else if (Value < 0) {
			FVector DeltaLocation = FVector(-GameplayCamera->GetCameraMovementSpeed(), 0.f, 0.f);
			if (GameplayCamera->GetCameraBoom()) GameplayCamera->GetCameraBoom()->AddRelativeLocation(DeltaLocation);
		}
	}
}
#pragma endregion