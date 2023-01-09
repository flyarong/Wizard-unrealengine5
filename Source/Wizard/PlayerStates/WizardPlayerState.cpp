// Fill out your copyright notice in the Description page of Project Settings.


#include "WizardPlayerState.h"
#include "Wizard/WizardCharacter.h"
#include "Net/UnrealNetwork.h"

void AWizardPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWizardPlayerState, CharacterName);
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
