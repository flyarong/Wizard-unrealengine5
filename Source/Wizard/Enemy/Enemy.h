// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Wizard/Interfaces/WizardCombatActor.h"
#include "Wizard/WizardTypes/CombatTypes.h"
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
	virtual float GetCost() override;
	virtual int32 GetHealth() override;
	virtual void Kill() override;
	virtual ECombat GetCombatType() override;

protected:
	virtual void BeginPlay() override;

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
#pragma endregion

#pragma region Callbacks
	UFUNCTION()
	void OnEnemyClicked(
		UPrimitiveComponent* TouchedComp,
		FKey ButtonPressed
	);
#pragma endregion
};
