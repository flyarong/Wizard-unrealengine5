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
	/// Sphere which defines how close the Actor
	/// can be approached
	/// </summary>
	UPROPERTY(VisibleAnywhere)
	USphereComponent* BorderSphere;

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

	/// <summary>
	/// Timer to initiate when OnSeePawn is bound
	/// to a callback
	/// </summary>
	FTimerHandle SensingTimer;

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
