// Fill out your copyright notice in the Description page of Project Settings.


#include "WizardPlayerState.h"
#include "Wizard/Characters/WizardCharacter.h"
#include "Wizard/Controllers/WizardPlayerController.h"
#include "Wizard/GameModes/WizardGameMode.h"
#include "Net/UnrealNetwork.h"

void AWizardPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWizardPlayerState, CharacterName);
	DOREPLIFETIME(AWizardPlayerState, Actions);
}

void AWizardPlayerState::SetSelectedCharacter(FName RowName)
{
	CharacterName = RowName;

	AWizardCharacter* WizardCharacter = Cast<AWizardCharacter>(GetPawn());
	if (WizardCharacter) {
		WizardCharacter->InitGameplayCharacter(GetPlayerName(), CharacterName);
	}
}

void AWizardPlayerState::OnRep_SelectedCharacter()
{
	AWizardCharacter* WizardCharacter = Cast<AWizardCharacter>(GetPawn());
	if (WizardCharacter) {
		WizardCharacter->InitGameplayCharacter(GetPlayerName(), CharacterName);
	}
}

void AWizardPlayerState::SpendAction(EAction Action)
{
	ServerCalculateAction(Action);

	Controller = Controller == nullptr ? Cast<AWizardPlayerController>(GetPawn()->Controller) : Controller;
	if (Controller) {
		Controller->SetHUDActions(Actions);
	}
}

void AWizardPlayerState::OnRep_Actions()
{
	Controller = Controller == nullptr ? Cast<AWizardPlayerController>(GetPawn()->Controller) : Controller;
	if (Controller) {
		Controller->SetHUDActions(Actions);
	}
}

void AWizardPlayerState::ServerCalculateAction_Implementation(EAction Action)
{
	AWizardGameMode* GameMode = Cast<AWizardGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode) {
		int32 Cost = GameMode->GetActionCost(Action);
		Actions = FMath::Clamp(Actions - Cost, 0, NumOfActionsPerRound);
	}
}