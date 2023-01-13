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

	AWizardPlayerState* WizardPlayerState = NewPlayer->GetPlayerState<AWizardPlayerState>();
	AWizardPlayerController* PlayerController = Cast<AWizardPlayerController>(NewPlayer);
	if (PlayerController && WizardPlayerState) {
		FName PlayerCharacter = GetPlayerCharacter(WizardPlayerState->GetPlayerName());
		PlayerController->InitCharacter(PlayerCharacter);
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

int32 AWizardGameMode::GetActionCost(EAction Action)
{
	switch (Action)
	{
	case EAction::EA_Movement:
		return 1;
		break;
	default:
		return 0;
		break;
	}
}
