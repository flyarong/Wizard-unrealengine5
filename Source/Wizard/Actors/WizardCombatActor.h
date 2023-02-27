// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WizardActor.h"
#include "WizardCombatActor.generated.h"

/**
 * Wizard Actor for Combat
 */
UCLASS()
class WIZARD_API AWizardCombatActor : public AWizardActor
{
	GENERATED_BODY()
	
public:

	/// <summary>
	/// Function to damage the Actor after Combat
	/// </summary>
	virtual void DamageActor(int32 Damage);

protected:

	/// <summary>
	/// Cost of the Actor interaction
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Cost")
	float Cost = 0.f;

	/// <summary>
	/// Actor's Health
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 Health = 1;

public:
	FORCEINLINE float GetCost() const { return Cost; }
	FORCEINLINE int32 GetHealth() const { return Health; }
};
