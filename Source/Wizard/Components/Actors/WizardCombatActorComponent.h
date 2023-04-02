// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Wizard/WizardTypes/CombatTypes.h"
#include "WizardCombatActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WIZARD_API UWizardCombatActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWizardCombatActorComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/// <summary>
	/// Function to setup this component's Owner and
	/// event bindings
	/// </summary>
	/// <param name="OwningActor">Owning Actor</param>
	/// <param name="Sphere">Owning Actor's AreaSphere</param>
	void SetupComponent(class AActor* OwningActor, class USphereComponent* Sphere);

	/// <summary>
	/// Function that sets up the Events and Callbacks for attacking
	/// against WizardCharacters
	/// </summary>
	void SetupAttackEvents();

	/// <summary>
	/// Function that sets up the Events and Callbacks for defending
	/// against WizardCharacters
	/// </summary>
	void SetupDefendEvents();

	/// <summary>
	/// Function to handle attacking a Wizard
	/// Character
	/// </summary>
	/// <param name="WCharacter">Attacked Character</param>
	void AttackCharacter(AActor* WCharacter);

	/// <summary>
	/// Function to damage the Actor after Combat
	/// </summary>
	void ReceiveDamage(int32 Damage);

	/// <summary>
	/// Function that gets the amount of damage to apply to
	/// a Character based on its Combat Score
	/// </summary>
	/// <param name="CharacterScore">Character's Combat Score</param>
	/// <returns>Damage Amount to apply on the Character</returns>
	float GetDamage(int32 CharacterScore);

	/// <summary>
	/// Callback function to spawn Item(s)
	/// after this Actor is destroyed
	/// </summary>
	UFUNCTION()
	void SpawnPickupItem(AActor* DestroyedActor);

protected:
	virtual void BeginPlay() override;

private:

	/// <summary>
	/// This component's Owning Actor
	/// </summary>
	UPROPERTY()
	AActor* Owner;

	/// <summary>
	/// Pointer to the Character the Owner
	/// has attacked
	/// </summary>
	UPROPERTY()
	class AWizardCharacter* AttackedCharacter;

	/// <summary>
	/// Pointer to the Character attacking
	/// the Owner
	/// </summary>
	UPROPERTY()
	AWizardCharacter* Attacker;

	/// <summary>
	/// Owner's AreaSphere
	/// </summary>
	UPROPERTY()
	class USphereComponent* AreaSphere;

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

	/// <summary>
	/// Actor's Base Damage: used in Character
	/// Defense Combat
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 BaseDamage = 1;

	/// <summary>
	/// Actor's Damage Amount per Score
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Combat")
	float DamageAmount = 10.f;

	/// <summary>
	/// Actor's Combat Type
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Combat")
	ECombat CombatType;

	/// <summary>
	/// Radius of how close the Owner
	/// can be approached by the Attacker
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Combat")
	float ApproachRadius = 175.f;

	/// <summary>
	/// Function for determining whether or not the Character
	/// is close enough to the Owner
	/// </summary>
	/// <returns>Whether or not the Character is close enough</returns>
	bool IsCharacterClose();

#pragma region Callbacks
	UFUNCTION()
	void OnActorBeginDefenseOverlap(
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

	/// <summary>
	/// Callback function to when the AttackedCharacter
	/// has ended their Defense Combat
	/// </summary>
	/// <param name="CharacterAttacked">Character attacked by Owner</param>
	/// <param name="NewAttacker">The new Attacker in line</param>
	UFUNCTION()
	void OnCharacterEndedDefense(AWizardCharacter* CharacterAttacked, AActor* NewAttacker);
#pragma endregion

	// <summary>
	/// Boolean for whether the Actor
	/// can be interacted with or it is already
	/// being interacted with by another Player
	/// </summary>
	UPROPERTY(Replicated)
	bool bCanInteract = true;		

public:
	FORCEINLINE void SetCanInteract(bool bIsInteractable) { bCanInteract = bIsInteractable; }
	FORCEINLINE bool GetCanInteract() const { return bCanInteract; }
	FORCEINLINE float GetCost() const { return Cost; }
	FORCEINLINE int32 GetHealth() const { return Health; }
	FORCEINLINE ECombat GetCombatType() const { return CombatType; }
	FORCEINLINE int32 GetBaseDamage() const { return BaseDamage; }
};
