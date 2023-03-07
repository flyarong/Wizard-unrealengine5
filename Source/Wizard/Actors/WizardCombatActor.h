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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/// <summary>
	/// Function to set whether or not the
	/// Actor can be interacted with
	/// </summary>
	void SetCanInteract(bool bIsInteractable);

protected:
	virtual void BeginPlay() override;

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

#pragma region Callbacks
	/// <summary>
	/// Callback function to spawn Item(s)
	/// after this Actor is destroyed
	/// </summary>
	UFUNCTION()
	void SpawnPickupItem(AActor* DestroyedActor);

	UFUNCTION()
	void OnActorBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnActorEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);
#pragma endregion

	// <summary>
	/// Boolean for whether the Actor
	/// can be interacted with or it is already
	/// being interacted with by another Player
	/// </summary>
	UPROPERTY(Replicated)
	bool bCanInteract = true;

public:
	FORCEINLINE bool GetCanInteract() const { return bCanInteract; }
	FORCEINLINE float GetCost() const { return Cost; }
	FORCEINLINE int32 GetHealth() const { return Health; }
};
