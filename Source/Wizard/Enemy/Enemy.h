// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Wizard/Interfaces/WizardCombatActor.h"
#include "Wizard/WizardTypes/CombatTypes.h"
#include "Wizard/WizardTypes/AttributeTypes.h"
#include "Enemy.generated.h"

UCLASS()
class WIZARD_API AEnemy : public ACharacter, public IWizardCombatActor
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual class UTexture2D* GetIcon() override;
	virtual void ShowInteractWidget(bool bShowInteractWidget) override;
	virtual void SetCanInteract(bool bIsInteractable) override;
	virtual bool GetCanInteract() override;
	virtual void ReceiveDamage(int32 Damage) override;
	virtual int32 GetBaseDamage() override;
	virtual float GetDamage(int32 CharacterScore) override;
	virtual float GetCost() override;
	virtual int32 GetHealth() override;
	virtual void Kill() override;
	virtual ECombat GetCombatType() override;
	virtual void MoveCombatActor() override;
	virtual void SetupActorForDefense() override;
	virtual void SetupActorForAttack() override;

	/// <summary>
	/// Callback function to end attacking
	/// </summary>
	UFUNCTION()
	void OnAttackEnded(class AActor* EnemyActor);

protected:
	virtual void BeginPlay() override;

	/// <summary>
	/// Controller of the Enemy Character
	/// </summary>
	class AAIController* EnemyController;

	/// <summary>
	/// Function to move the Enemy when it
	/// sees no Wizards
	/// </summary>
	UFUNCTION()
	void MoveEnemyToUnseenWizard();

	/// <summary>
	/// Function to choose an Attribute for the Enemy to base
	/// its movement on
	/// </summary>
	/// <returns>Character Attribute to focus on</returns>
	virtual EAttribute GetChosenAttribute();

private:
#pragma region Components
	/// <summary>
	/// Actor's Point of Interest component:
	/// shows the location of the Actor on the MiniMap
	/// </summary>
	UPROPERTY(VisibleAnywhere)
	class UPointOfInterestComponent* POI;

	/// <summary>
	/// Combat Component to be able initiate 
	/// Combat with the Actor
	/// </summary>
	UPROPERTY(VisibleAnywhere)
	class UWizardCombatActorComponent* Combat;

	/// <summary>
	/// AreaSphere to interact with the Actor
	/// </summary>
	UPROPERTY(VisibleAnywhere)
	class USphereComponent* AreaSphere;

	/// <summary>
	/// Interact widget component
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UInteractComponent* InteractComponent;

	/// <summary>
	/// Pawn sensing component
	/// </summary>
	UPROPERTY(VisibleAnywhere)
	class UPawnSensingComponent* PawnSensing;
#pragma endregion

	/// <summary>
	/// Target of Combat
	/// </summary>
	UPROPERTY()
	class APawn* TargetCharacter;

	/// <summary>
	/// Function to call when attack has ended
	/// </summary>
	void EndAttack();

	/// <summary>
	/// Number of seconds the Enemy can move
	/// towards an unseen Wizard
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Enemy Movement")
	float MovementTime = 5.f;

	/// <summary>
	/// Attritube to base the chasing
	/// of an unseen Wizard on
	/// </summary>
	UPROPERTY()
	EAttribute ChosenAttributeToChase;

	/// <summary>
	/// Function to move to a Wizard Character
	/// </summary>
	/// <param name="TargetWizard">Target Wizard to move to</param>
	void MoveToWizard(class AActor* TargetWizard);

	UPROPERTY(EditAnywhere, Category = "Enemy Movement")
	float CombatRadius = 400.f;

	/// <summary>
	/// Function determining whether or not
	/// a CombatTarget for this Enemy is in
	/// Range or not
	/// </summary>
	/// <param name="Target">Combat Target</param>
	/// <param name="Radius">Combat Radius</param>
	/// <returns>Whether the Target is in Range or not</returns>
	bool InTargetRange(AActor* Target, float Radius);

	/// <summary>
	/// Timer to initiate when OnSeePawn is bound
	/// to a callback
	/// </summary>
	FTimerHandle SensingTimer;

	/// <summary>
	/// Timer to initiate when moving towards
	/// an unseen Character
	/// </summary>
	FTimerHandle MovementTimer;

#pragma region Callbacks
	UFUNCTION()
	void OnEnemyClicked(
		UPrimitiveComponent* TouchedComp,
		FKey ButtonPressed
	);

	/// <summary>
	/// Callback function to when the Enemy
	/// sees a Wizard
	/// </summary>
	/// <param name="Pawn">Pawn seen</param>
	UFUNCTION()
	void OnSeeWizard(class APawn* Pawn);

	/// <summary>
	/// Callback function to stop the
	/// Enemy movement when movement timer
	/// finishes
	/// </summary>
	UFUNCTION()
	void StopEnemyMovement();
#pragma endregion
};
