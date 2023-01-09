// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameState.h"
#include "Net/UnrealNetwork.h"
#include "Wizard/Controllers/LobbyController.h"

void ALobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALobbyGameState, SelectionStatus);
	DOREPLIFETIME(ALobbyGameState, BusyPlayers);
}

void ALobbyGameState::UpdateCharacterSelection(int32 PreviousIndex, int32 NewIndex)
{
	if (PreviousIndex >= 0) SelectionStatus[PreviousIndex] = true;
	if (NewIndex >= 0) SelectionStatus[NewIndex] = false;

	ALobbyController* Controller = Cast<ALobbyController>(GetWorld()->GetFirstPlayerController());
	if (Controller)
	{
		Controller->UpdateHUDCharacterSelector();
	}
}

void ALobbyGameState::OnRep_UpdateCharacterSelection()
{
	ALobbyController* Controller = Cast<ALobbyController>(GetWorld()->GetFirstPlayerController());
	if (Controller)
	{
		Controller->UpdateHUDCharacterSelector();
	}
}

void ALobbyGameState::UpdatePlayerList(FString PlayerToAdd, FString PlayerToRemove)
{
	if (!PlayerToAdd.IsEmpty()) BusyPlayers.Add(PlayerToAdd);
	if (!PlayerToRemove.IsEmpty() && BusyPlayers.Contains(PlayerToRemove)) BusyPlayers.Remove(PlayerToRemove);

	ALobbyController* Controller = Cast<ALobbyController>(GetWorld()->GetFirstPlayerController());
	if (Controller)
	{
		Controller->UpdateHUDPlayerList(BusyPlayers);
	}
}

void ALobbyGameState::OnRep_UpdatePlayerList()
{
	ALobbyController* Controller = Cast<ALobbyController>(GetWorld()->GetFirstPlayerController());
	if (Controller)
	{
		Controller->UpdateHUDPlayerList(BusyPlayers);
	}
}
