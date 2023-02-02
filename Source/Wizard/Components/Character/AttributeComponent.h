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

	/// <summary>
	/// Function to handle Energy expenditure
	/// </summary>
	/// <param name="Cost">The number of energy to be lost</param>
	void SpendEnergy(float Cost);

	/// <summary>
	/// Function to handle XP expenditure
	/// </summary>
	/// <param name="Cost">The number of XP to be spent</param>
	UFUNCTION(Server, Reliable)
	void ServerSpendXP(int32 Cost);

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

#pragma region XP
	UPROPERTY(ReplicatedUsing = OnRep_XP, EditAnywhere, Category = "Wizard Attributes")
	int32 XP = 20;

	UFUNCTION()
	void OnRep_XP();

	/// <summary>
	/// Function to update the XP
	/// on the HUD
	/// </summary>
	void UpdateHUDXP();
#pragma endregion

#pragma region Energy
	UPROPERTY(ReplicatedUsing = OnRep_Energy, VisibleAnywhere, Category = "Wizard Attributes")
	float Energy;

	UFUNCTION()
	void OnRep_Energy();

	UPROPERTY(EditAnywhere, Category = "Wizard Attributes")
	float MaxEnergy = 100.f;

	/// <summary>
	/// Function to update the Energy bar
	/// on the HUD
	/// </summary>
	void UpdateHUDEnergy();
#pragma endregion

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
	FORCEINLINE FString GetName() const { return Name; }
	FORCEINLINE void SetHealth(int32 HealthToSet) { Health = HealthToSet; }
	FORCEINLINE int32 GetHealth() const { return Health; }
	FORCEINLINE void SetPower(int32 PowerToSet) { Power = PowerToSet; }
	FORCEINLINE int32 GetPower() const { return Power; }
	FORCEINLINE void SetWisdom(int32 WisdomToSet) { Wisdom = WisdomToSet; }
	FORCEINLINE int32 GetWisdom() const { return Wisdom; }
	FORCEINLINE void SetIntelligence(int32 IntelligenceToSet) { Intelligence = IntelligenceToSet; }
	FORCEINLINE int32 GetIntelligence() const { return Intelligence; }
	FORCEINLINE void SetCombat(int32 CombatToSet) { Combat = CombatToSet; }
	FORCEINLINE int32 GetCombat() const { return Combat; }
	FORCEINLINE void SetAgility(int32 AgilityToSet) { Agility = AgilityToSet; }
	FORCEINLINE int32 GetAgility() const { return Agility; }
	FORCEINLINE float GetEnergy() const { return Energy; }
	FORCEINLINE float GetMaxEnergy() const { return MaxEnergy; }
	FORCEINLINE int32 GetXP() const { return XP; }
	/** Whether the player has enough XP to handle the Cost of an action */
	FORCEINLINE bool HasEnoughXP(int32 Cost) const { return (XP - Cost) >= 0; }
		
};
