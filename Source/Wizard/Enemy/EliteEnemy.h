// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "EliteEnemy.generated.h"

/**
 * Character class for Elite Enemies
 */
UCLASS()
class WIZARD_API AEliteEnemy : public AEnemy
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

private:

	/// <summary>
	/// Multiplier to increase the Base Health
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Elite Properties")
	int32 HealthMultiplier = 1;

	/// <summary>
	/// Multiplier to increase the Base Damage
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Elite Properties")
	int32 DamageMultiplier = 1;
};
