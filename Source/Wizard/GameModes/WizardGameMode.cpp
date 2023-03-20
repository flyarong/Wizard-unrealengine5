// Copyright Epic Games, Inc. All Rights Reserved.

#include "WizardGameMode.h"
#include "Wizard/Controllers/WizardPlayerController.h"
#include "Wizard/GameInstance/WizardGameInstance.h"
#include "Wizard/PlayerStates/WizardPlayerState.h"
#include "Wizard/Characters/WizardCharacter.h"
#include "Wizard/Components/Character/AttributeComponent.h"
#include "Wizard/Interfaces/WizardCombatActor.h"
#include "GameFramework/PlayerState.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

namespace MatchState
{
	const FName Enemy = FName("Enemy");
	const FName Trial = FName("Trial");
	const FName Prepare = FName("Prepare");
}

AWizardGameMode::AWizardGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AWizardPlayerController::StaticClass();
	bDelayedStart = true;
}

void AWizardGameMode::BeginPlay()
{
	Super::BeginPlay();

	LevelStartingTime = GetWorld()->GetTimeSeconds();
}

void AWizardGameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();

	if (MatchState == MatchState::InProgress) {
		if (WizardPlayers.Num() > 0 && !bPlayersInitialized) {
			for (auto& WizardPlayer : WizardPlayers) {
				InitCharacter(WizardPlayer);
			}
			bPlayersInitialized = true;
		}

	}
}

void AWizardGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MatchState == MatchState::WaitingToStart)
	{
		CountdownTime = WarmupTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f)
		{
			StartMatch();
		}
	}
}

void AWizardGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	AWizardPlayerController* PlayerController = Cast<AWizardPlayerController>(NewPlayer);
	if (PlayerController) {
		PlayerController->ClientInitOverlay();
		WizardPlayers.Add(PlayerController);
	}

	if (MatchState == MatchState::InProgress && PlayerController) InitCharacter(PlayerController);
}

void AWizardGameMode::InitCharacter(AWizardPlayerController* Controller)
{
	AWizardPlayerState* WizardPlayerState = Controller->GetPlayerState<AWizardPlayerState>();
	if (WizardPlayerState) {
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

void AWizardGameMode::RemoveMiniMapActor(AActor* MiniMapActor)
{
	if (MiniMapActors.Contains(MiniMapActor)) {
		MiniMapActors.Remove(MiniMapActor);
	}
}

void AWizardGameMode::BroadcastChatMessage(const FText& Message)
{
	for (auto& WizardPlayer : WizardPlayers)
	{
		if (WizardPlayer)
		{
			WizardPlayer->ClientAddHUDChatMessage(Message);
		}
	}
}

void AWizardGameMode::BroadcastVictory(AWizardCharacter* WCharacter, const TScriptInterface<IWizardCombatActor>& CombatTarget)
{
	for (auto& WizardPlayer : WizardPlayers)
	{
		if (WizardPlayer)
		{
			WizardPlayer->ClientAddHUDVictoryPublicMessage(WCharacter, CombatTarget);
		}
	}
}

AWizardCharacter* AWizardGameMode::GetCharacterWithLowestAttribute(EAttribute AttributeType)
{
	if (WizardPlayers.Num() > 0 && WizardPlayers[0] && WizardPlayers[0]->GetWizardCharacter() && WizardPlayers[0]->GetWizardCharacter()->GetAttribute()) {
		int32 LowestValue = WizardPlayers[0]->GetWizardCharacter()->GetAttribute()->GetAttributeValue(AttributeType);
		int32 AttrValue = -1;
		TArray<AWizardCharacter*> WCs = TArray<AWizardCharacter*>();
		WCs.Add(WizardPlayers[0]->GetWizardCharacter());
		for (int32 i = 1; i < WizardPlayers.Num(); i++) {
			if (WizardPlayers[i]) {
				AWizardCharacter* WC = WizardPlayers[i]->GetWizardCharacter();
				if (WC && WC->GetAttribute()) {
					AttrValue = WC->GetAttribute()->GetAttributeValue(AttributeType);
					if (AttrValue <= LowestValue) {
						LowestValue = AttrValue;
						if (AttrValue < LowestValue) WCs = TArray<AWizardCharacter*>();
						WCs.Add(WC);
					}
				}
			}
		}

		if (WCs.Num() > 0) {
			int32 CharacterIndex = FMath::RandRange(0, WCs.Num() - 1);
			return WCs[CharacterIndex];
		}
	}

	return nullptr;
}

void AWizardGameMode::IncrementPlayersFinished()
{
	PlayersFinished++;
	if (PlayersFinished >= WizardPlayers.Num()) {
		SetMatchState(MatchState::Enemy);
	}
}

void AWizardGameMode::DecrementPlayersFinished()
{
	PlayersFinished--;
}
