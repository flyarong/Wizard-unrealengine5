// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

/**
* Actor class for spawning Enemies
*/
UCLASS()
class WIZARD_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemySpawner();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:

	/// <summary>
	/// GameState pointer
	/// </summary>
	class AWizardGameState* WGameState;

};
