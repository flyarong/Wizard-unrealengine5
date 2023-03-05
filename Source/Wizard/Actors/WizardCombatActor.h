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
	virtual void ReceiveDamage(int32 Damage);

protected:
	virtual void BeginPlay() override;

	/// <summary>
	/// Callback function to spawn Item(s)
	/// after this Actor is destroyed
	/// </summary>
	UFUNCTION()
	void SpawnPickupItem(AActor* DestroyedActor);

	/// <summary>
	/// Item classes that can be spawned and picked up
	/// after this Actor is destroyed
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Pickups")
	TArray<TSubclassOf<class AItem>> PickupClasses;

	/// <summary>
	/// Variable to determine how many Items
	/// should be spawned after this Actor
	/// is destroyed
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Pickups")
	int32 NumOfSelections = 1;

	/// <summary>
	/// Cost of the Actor interaction
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Cost")
	float Cost = 25.f;

	/// <summary>
	/// Actor's Health
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 Health = 1;

public:
	FORCEINLINE float GetCost() const { return Cost; }
	FORCEINLINE int32 GetHealth() const { return Health; }
};
