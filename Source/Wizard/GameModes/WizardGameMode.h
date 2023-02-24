// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "WizardGameMode.generated.h"

UCLASS(minimalapi)
class AWizardGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AWizardGameMode();
	virtual void Tick(float DeltaTime) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

	/// <summary>
	/// Function to add a new Actor to the
	/// MiniMapActors array
	/// </summary>
	/// <param name="MiniMapActor">New MiniMap Actor</param>
	void AddMiniMapActor(class AActor* MiniMapActor);

	/// <summary>
	/// Function to remove an Actor from the
	/// MiniMapActors array
	/// </summary>
	/// <param name="MiniMapActor">Actor to remove</param>
	void RemoveMiniMapActor(AActor* MiniMapActor);

	/// <summary>
	/// Function to Broadcast the Chat Message
	/// to all machines
	/// </summary>
	/// <param name="Message">The message to broadcast</param>
	void BroadcastChatMessage(const FText& Message);

	/// <summary>
	/// Function to broadcast the Public Message
	/// about a Character's victory in Combat
	/// </summary>
	/// <param name="WCharacter">The Character</param>
	/// <param name="CombatTarget">The Actor defeated in Combat</param>
	void BroadcastVictory(class AWizardCharacter* WCharacter, class AWizardActor* CombatTarget);

protected:
	virtual void BeginPlay() override;

private:

	/// <summary>
	/// Game Instance pointer
	/// </summary>
	class UWizardGameInstance* WizardGameInstance;

	/// <summary>
	/// Array of Game specific Player Controllers
	/// </summary>
	TArray<class AWizardPlayerController*> WizardPlayers;

	/// <summary>
	/// Function to initialize the joining player's character
	/// </summary>
	/// <param name="Controller">The player's controller</param>
	void InitCharacter(APlayerController* Controller);

	/// <summary>
	/// Array holding all the Actors that should be
	/// displayed on the MiniMap
	/// </summary>
	UPROPERTY()
	TArray<AActor*> MiniMapActors = TArray<AActor*>();

public:

	/// <summary>
	/// Get all the Actors present on the MiniMap
	/// </summary>
	/// <returns>Actors on MiniMap</returns>
	TArray<AActor*> GetMiniMapActors() const { return MiniMapActors; }

	/// <summary>
	/// Public getter for the player's character name
	/// </summary>
	/// <param name="PlayerName">Player's name</param>
	/// <returns>Character's name selected by the player</returns>
	FName GetPlayerCharacter(FString PlayerName);
};



