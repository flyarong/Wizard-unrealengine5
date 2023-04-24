// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Wizard/WizardTypes/ActionTypes.h"
#include "Wizard/WizardTypes/AttributeTypes.h"
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
	/// Function to receive Damage on Health
	/// </summary>
	/// <param name="Damage">Damage amount</param>
	void ReceiveDamage(float Damage);

	/// <summary>
	/// Function to add more Health
	/// </summary>
	/// <param name="AmountToAdd">Amount of Health to add</param>
	void AddHealth(int32 AmountToAdd);

	/// <summary>
	/// Function to handle Power expenditure
	/// </summary>
	/// <param name="Cost">The number of Power to be lost</param>
	/// <param name="ActionType">Action type the Cost is calculated based on</param>
	void SpendPower(float Cost, EAction ActionType);

	/// <summary>
	/// Function to add more Power
	/// </summary>
	/// <param name="AmountToAdd">Amount of Power to add</param>
	void AddPower(int32 AmountToAdd);

	/// <summary>
	/// Function to handle XP expenditure
	/// </summary>
	/// <param name="Cost">The number of XP to be spent</param>
	void SpendXP(int32 Cost);

	/// <summary>
	/// Function to add more XP
	/// </summary>
	/// <param name="AmountToAdd">Amount of XP to add</param>
	void AddXP(int32 AmountToAdd);

	/// <summary>
	/// Function to handle GoodSpell expenditure
	/// </summary>
	/// <param name="Cost">The number of GoodSpells to be spent</param>
	void SpendGoodSpell(int32 Cost);

	/// <summary>
	/// Function to add more GoodSpell
	/// </summary>
	/// <param name="GoodSpellAmount">Amount to add</param>
	void AddGoodSpell(int32 GoodSpellAmount);

	/// <summary>
	/// Function to handle DarkSpell expenditure
	/// </summary>
	/// <param name="Cost">The number of DarkSpells to be spent</param>
	void SpendDarkSpell(int32 Cost);

	/// <summary>
	/// Function to add more DarkSpell
	/// </summary>
	/// <param name="DarkSpellAmount">Amount to add</param>
	void AddDarkSpell(int32 DarkSpellAmount);

	/// <summary>
	/// Function to handle Wisdom decrease
	/// </summary>
	/// <param name="AmountToSubtract">The number of Wisdom to subtract</param>
	void DecreaseWisdom(int32 AmountToSubtract);

	/// <summary>
	/// Function to handle Wisdom increase
	/// </summary>
	/// <param name="AmountToAdd">The number of Wisdom to add</param>
	void IncreaseWisdom(int32 AmountToAdd);

	/// <summary>
	/// Function to handle Intelligence decrease
	/// </summary>
	/// <param name="AmountToSubtract">The number of Intelligence to subtract</param>
	void DecreaseIntelligence(int32 AmountToSubtract);

	/// <summary>
	/// Function to handle Intelligence increase
	/// </summary>
	/// <param name="AmountToAdd">The number of Intelligence to add</param>
	void IncreaseIntelligence(int32 AmountToAdd);

	/// <summary>
	/// Function to handle Offense decrease
	/// </summary>
	/// <param name="AmountToSubtract">The number of Offense to subtract</param>
	void DecreaseOffense(int32 AmountToSubtract);

	/// <summary>
	/// Function to handle Offense increase
	/// </summary>
	/// <param name="AmountToAdd">The number of Offense to add</param>
	void IncreaseOffense(int32 AmountToAdd);

	/// <summary>
	/// Function to handle Defense decrease
	/// </summary>
	/// <param name="AmountToSubtract">The number of Defense to subtract</param>
	void DecreaseDefense(int32 AmountToSubtract);

	/// <summary>
	/// Function to handle Defense increase
	/// </summary>
	/// <param name="AmountToAdd">The number of Defense to add</param>
	void IncreaseDefense(int32 AmountToAdd);

	/// <summary>
	/// Function to handle Agility decrease
	/// </summary>
	/// <param name="AmountToSubtract">The number of Agility to subtract</param>
	void DecreaseAgility(int32 AmountToSubtract);

	/// <summary>
	/// Function to handle Agility increase
	/// </summary>
	/// <param name="AmountToAdd">The number of Agility to add</param>
	void IncreaseAgility(int32 AmountToAdd);

	/// <summary>
	/// Function that gets the value of an
	/// Attribute based on its Type
	/// </summary>
	/// <param name="AttributeType">Type of the Attribute</param>
	/// <returns>Attribute value</returns>
	int32 GetAttributeValue(EAttribute AttributeType);

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

#pragma region Health
	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category = "Wizard Attributes")
	float Health;

	UFUNCTION()
	void OnRep_Health();

	UPROPERTY(EditAnywhere, Category = "Wizard Attributes")
	float MaxHealth = 100.f;

	/// <summary>
	/// Function to update the Health bar
	/// on the HUD
	/// </summary>
	void UpdateHUDHealth();
#pragma endregion

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

#pragma region Power
	UPROPERTY(ReplicatedUsing = OnRep_Power, VisibleAnywhere, Category = "Wizard Attributes")
	float Power;

	UFUNCTION()
	void OnRep_Power();

	UPROPERTY(EditAnywhere, Category = "Wizard Attributes")
	float MaxPower = 100.f;

	/// <summary>
	/// Function to update the Power bar
	/// on the HUD
	/// </summary>
	void UpdateHUDPower();
#pragma endregion

#pragma region Spells
	UPROPERTY(ReplicatedUsing = OnRep_GoodSpells, EditAnywhere, Category = "Wizard Attributes")
	int32 GoodSpells = 0;

	UFUNCTION()
	void OnRep_GoodSpells();

	/// <summary>
	/// Function to update the GoodSpells
	/// on the HUD
	/// </summary>
	void UpdateHUDGoodSpells();

	UPROPERTY(ReplicatedUsing = OnRep_DarkSpells, EditAnywhere, Category = "Wizard Attributes")
	int32 DarkSpells = 0;

	UFUNCTION()
	void OnRep_DarkSpells();

	/// <summary>
	/// Function to update the DarkSpells
	/// on the HUD
	/// </summary>
	void UpdateHUDDarkSpells();
#pragma endregion

#pragma region Wisdom
	UPROPERTY(ReplicatedUsing = OnRep_Wisdom, EditAnywhere, Category = "Wizard Attributes")
	int32 Wisdom;

	UFUNCTION()
	void OnRep_Wisdom();

	/// <summary>
	/// Function to update the Wisdom
	/// on the HUD
	/// </summary>
	void UpdateHUDWisdom();
#pragma endregion

#pragma region Intelligence
	UPROPERTY(ReplicatedUsing = OnRep_Intelligence, EditAnywhere, Category = "Wizard Attributes")
	int32 Intelligence;

	UFUNCTION()
	void OnRep_Intelligence();

	/// <summary>
	/// Function to update the Intelligence
	/// on the HUD
	/// </summary>
	void UpdateHUDIntelligence();
#pragma endregion

#pragma region Offense
	UPROPERTY(ReplicatedUsing = OnRep_Offense, EditAnywhere, Category = "Wizard Attributes")
	int32 Offense;

	UFUNCTION()
	void OnRep_Offense();

	/// <summary>
	/// Function to update the Offense
	/// on the HUD
	/// </summary>
	void UpdateHUDOffense();
#pragma endregion

#pragma region Defense
	UPROPERTY(ReplicatedUsing = OnRep_Defense, EditAnywhere, Category = "Wizard Attributes")
	int32 Defense;

	UFUNCTION()
	void OnRep_Defense();

	/// <summary>
	/// Function to update the Defense
	/// on the HUD
	/// </summary>
	void UpdateHUDDefense();
#pragma endregion

#pragma region Agility
	UPROPERTY(ReplicatedUsing = OnRep_Agility, EditAnywhere, Category = "Wizard Attributes")
	int32 Agility;

	UFUNCTION()
	void OnRep_Agility();

	/// <summary>
	/// Function to update the Agility
	/// on the HUD
	/// </summary>
	void UpdateHUDAgility();
#pragma endregion

public:	
	FORCEINLINE void SetName(FString NameToSet) { Name = NameToSet; }
	FORCEINLINE FString GetName() const { return Name; }
	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	FORCEINLINE int32 GetGoodSpells() const { return GoodSpells; }
	FORCEINLINE int32 GetDarkSpells() const { return DarkSpells; }
	FORCEINLINE void SetDefense(int32 DefenseToSet) { Defense = DefenseToSet; }
	FORCEINLINE int32 GetDefense() const { return Defense; }
	FORCEINLINE void SetWisdom(int32 WisdomToSet) { Wisdom = WisdomToSet; }
	FORCEINLINE int32 GetWisdom() const { return Wisdom; }
	FORCEINLINE void SetIntelligence(int32 IntelligenceToSet) { Intelligence = IntelligenceToSet; }
	FORCEINLINE int32 GetIntelligence() const { return Intelligence; }
	FORCEINLINE void SetOffense(int32 OffenseToSet) { Offense = OffenseToSet; }
	FORCEINLINE int32 GetOffense() const { return Offense; }
	FORCEINLINE void SetAgility(int32 AgilityToSet) { Agility = AgilityToSet; }
	FORCEINLINE int32 GetAgility() const { return Agility; }
	FORCEINLINE float GetPower() const { return Power; }
	FORCEINLINE float GetMaxPower() const { return MaxPower; }
	FORCEINLINE int32 GetXP() const { return XP; }
	/** Whether the player has enough XP to handle the Cost of an action */
	FORCEINLINE bool HasEnoughXP(int32 Cost) const { return (XP - Cost) >= 0; }
		
};
