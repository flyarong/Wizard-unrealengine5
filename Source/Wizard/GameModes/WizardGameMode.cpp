// Copyright Epic Games, Inc. All Rights Reserved.

#include "WizardGameMode.h"
#include "Wizard/Controllers/WizardPlayerController.h"
#include "Wizard/GameInstance/WizardGameInstance.h"
#include "Wizard/PlayerStates/WizardPlayerState.h"
#include "GameFramework/PlayerState.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

AWizardGameMode::AWizardGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AWizardPlayerController::StaticClass();
}

void AWizardGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AWizardGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWizardGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	InitCharacter(NewPlayer);
}

void AWizardGameMode::InitCharacter(APlayerController* Controller)
{
	AWizardPlayerController* PlayerController = Cast<AWizardPlayerController>(Controller);
	AWizardPlayerState* WizardPlayerState = Controller->GetPlayerState<AWizardPlayerState>();
	if (PlayerController && WizardPlayerState) {
		FName PlayerCharacter = GetPlayerCharacter(WizardPlayerState->GetPlayerName());
		if (PlayerCharacter.IsValid()) {

			// Set timer to wait for client Pawn to be valid
			FTimerHandle CharacterInitTimer;
			FTimerDelegate CharacterInitDelegate;
			CharacterInitDelegate.BindUFunction(WizardPlayerState, FName("SetSelectedCharacter"), PlayerCharacter);

			GetWorldTimerManager().SetTimer(
				CharacterInitTimer,
				CharacterInitDelegate,
				3.f,
				false
			);

		}
	}
}

FName AWizardGameMode::GetPlayerCharacter(FString PlayerName)
{
	WizardGameInstance = WizardGameInstance == nullptr ? 
		Cast<UWizardGameInstance>(UGameplayStatics::GetGameInstance(this)) : WizardGameInstance;
	if (WizardGameInstance && WizardGameInstance->SelectedCharacters.Num() > 0 && !PlayerName.IsEmpty()) {
		return *WizardGameInstance->SelectedCharacters.Find(PlayerName);
	}

	return FName("Lohion"); // NAME_None
}

void AWizardGameMode::AddMiniMapActor(AActor* MiniMapActor)
{
	if (MiniMapActor) MiniMapActors.AddUnique(MiniMapActor);
}
