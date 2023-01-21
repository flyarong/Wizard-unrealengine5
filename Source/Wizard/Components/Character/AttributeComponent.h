// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"

/**
* Component class to handle Character attribute functionality
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WIZARD_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttributeComponent();
	friend class AWizardCharacter;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SpendEnergy(float Cost);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	/// <summary>
	/// Pointer to the player character
	/// </summary>
	UPROPERTY()
	class AWizardCharacter* Character;

	/// <summary>
	/// Pointer to the player character
	/// </summary>
	UPROPERTY()
	class AWizardPlayerController* Controller;

	/**
	* Wizard Attributes
	*/

	UPROPERTY(Replicated, EditAnywhere, Category = "Wizard Attributes")
	FString Name;

	UPROPERTY(Replicated, EditAnywhere, Category = "Wizard Attributes")
	int32 Health;

	UPROPERTY(Replicated, EditAnywhere, Category = "Wizard Attributes")
	int32 Power;

	UPROPERTY(ReplicatedUsing = OnRep_Energy, VisibleAnywhere, Category = "Wizard Attributes")
	float Energy = 100.f;

	UFUNCTION()
	void OnRep_Energy();

	/// <summary>
	/// Function to update the Energy bar
	/// on the HUD
	/// </summary>
	void UpdateHUDEnergy();

	UPROPERTY(EditAnywhere, Category = "Wizard Attributes")
	float MaxEnergy = 100.f;

	UPROPERTY(Replicated, EditAnywhere, Category = "Wizard Attributes")
	int32 Wisdom;

	UPROPERTY(Replicated, EditAnywhere, Category = "Wizard Attributes")
	int32 Intelligence;

	UPROPERTY(Replicated, EditAnywhere, Category = "Wizard Attributes")
	int32 Combat;

	UPROPERTY(Replicated, EditAnywhere, Category = "Wizard Attributes")
	int32 Agility;

public:	
	FORCEINLINE void SetName(FString NameToSet) { Name = NameToSet; }
	FORCEINLINE void SetHealth(int32 HealthToSet) { Health = HealthToSet; }
	FORCEINLINE void SetPower(int32 PowerToSet) { Power = PowerToSet; }
	FORCEINLINE void SetWisdom(int32 WisdomToSet) { Wisdom = WisdomToSet; }
	FORCEINLINE void SetIntelligence(int32 IntelligenceToSet) { Intelligence = IntelligenceToSet; }
	FORCEINLINE void SetCombat(int32 CombatToSet) { Combat = CombatToSet; }
	FORCEINLINE void SetAgility(int32 AgilityToSet) { Agility = AgilityToSet; }
		
};
