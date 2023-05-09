// Copyright Epic Games, Inc. All Rights Reserved.

#include "WizardGameMode.h"
#include "Wizard/Controllers/WizardPlayerController.h"
#include "Wizard/GameInstance/WizardGameInstance.h"
#include "Wizard/GameStates/WizardGameState.h"
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

	WizardGameState = GetGameState<AWizardGameState>();
	if (WizardGameState) {
		WizardGameState->OnPrepareFinishedDelegate.BindUObject(this, &AWizardGameMode::OnPrepareStateFinished);
		WizardGameState->OnThresholdReached.BindUObject(this, &AWizardGameMode::OnThresholdReached);
	}

	LevelStartingTime = GetWorld()->GetTimeSeconds();
}

void AWizardGameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();

	PlayersFinished = 0;
	if (bPlayersInitialized) {
		for (auto& WizardPlayer : WizardPlayers) {
			WizardPlayer->OnMatchStateSet(MatchState);
		}
	}

	WizardGameState = WizardGameState == nullptr ? GetGameState<AWizardGameState>() : WizardGameState;

	if (MatchState == MatchState::InProgress && WizardGameState) {
		if (!bPlayersInitialized) {
			for (auto& WizardPlayer : WizardPlayers) {
				InitCharacter(WizardPlayer);
			}
			bPlayersInitialized = true;
		}
		else {
			for (auto& WizardPlayer : WizardPlayers) {
				if (WizardPlayer && WizardPlayer->GetWizardCharacter()) {
					WizardPlayer->GetWizardCharacter()->RefillPower();
				}
			}
		}
		
		WizardGameState->EnableWizardActors();
	}
	else if (MatchState == MatchState::Enemy && WizardGameState) {
		WizardGameState->DisableWizardActors(); // needs to run before MatchState widget animation ends
	}
}

void AWizardGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// TODO change this to prologue text on screen and add button to increment playersfinished
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

void AWizardGameMode::BroadcastStoryPointUpdate(const int32& NumOfPoints, bool bIsPositivePoints)
{
	for (auto& WizardPlayer : WizardPlayers)
	{
		if (WizardPlayer)
		{
			WizardPlayer->ClientSetHUDStoryPoints(NumOfPoints, bIsPositivePoints);
		}
	}
}

void AWizardGameMode::EndGame(bool bIsGameWon)
{
	SetMatchState(MatchState::WaitingPostMatch);

	for (auto& WizardPlayer : WizardPlayers) {
		WizardPlayer->OnGameEnded(bIsGameWon);
	}
}

void AWizardGameMode::PlayerLeavingGame(AWizardPlayerController* WController)
{
	if (WizardPlayers.Num() > 0 && WController && WizardPlayers.Contains(WController)) {
		WizardPlayers.Remove(WController);
	}
}

AWizardCharacter* AWizardGameMode::GetCharacterWithLowestAttribute(EAttribute AttributeType)
{
	if (WizardPlayers.Num() > 0) {
		int32 LowestValue = 99;
		int32 AttrValue = -1;
		TArray<AWizardCharacter*> WCs = TArray<AWizardCharacter*>();
		for (int32 i = 0; i < WizardPlayers.Num(); i++) {
			if (WizardPlayers[i]) {
				AWizardCharacter* WC = Cast<AWizardCharacter>(WizardPlayers[i]->GetPawn());
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

AWizardCharacter* AWizardGameMode::GetClosestCharacter(AActor* Enemy)
{
	AWizardCharacter* ClosestCharacter = nullptr;
	float ClosestDistance = 10000.f;
	float Distance = -1.f;
	if (WizardPlayers.Num() > 0 && Enemy) {
		for (int32 i = 0; i < WizardPlayers.Num(); i++) {
			if (WizardPlayers[i]) {
				AWizardCharacter* WC = Cast<AWizardCharacter>(WizardPlayers[i]->GetPawn());
				if (WC) {
					Distance = (WC->GetActorLocation() - Enemy->GetActorLocation()).Size();
					if (Distance < ClosestDistance) {
						ClosestDistance = Distance;
						ClosestCharacter = WC;
					}
				}
			}
		}
	}
	
	return ClosestCharacter;
}

void AWizardGameMode::IncrementPlayersFinished()
{
	PlayersFinished++;

	if (PlayersFinished >= WizardPlayers.Num()) {
		FName NextState = MatchState::InProgress;
		if (MatchState == MatchState::InProgress) NextState = MatchState::Enemy;
		else if (MatchState == MatchState::Trial) NextState = MatchState::Prepare;
		SetMatchState(NextState);
	}
}

void AWizardGameMode::DecrementPlayersFinished()
{
	PlayersFinished--;
}

void AWizardGameMode::IncrementEnemiesFinished()
{
	WizardGameState = WizardGameState == nullptr ? GetGameState<AWizardGameState>() : WizardGameState;

	if (WizardGameState) {
		WizardGameState->IncrementEnemies();
		if (WizardGameState->EnemiesFinishedTheirTurn()) {
			SetMatchState(MatchState::Trial);
		}
	}
}

void AWizardGameMode::OnPrepareStateFinished()
{
	SetMatchState(MatchState::InProgress);
}

void AWizardGameMode::OnThresholdReached()
{
	for (auto& WizardPlayer : WizardPlayers)
	{
		if (WizardPlayer)
		{
			// TODO client rpc to show notification
		}
	}
}
