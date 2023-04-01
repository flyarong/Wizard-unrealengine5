// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Wizard/Interfaces/WizardCombatActor.h"
#include "Wizard/WizardTypes/CombatTypes.h"
#include "Spell.generated.h"

/**
* Spell Actor base class
*/
UCLASS()
class WIZARD_API ASpell : public AActor, public IWizardCombatActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpell();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;
	virtual class UTexture2D* GetIcon() override;
	virtual void ShowInteractWidget(bool bShowInteractWidget) override;
	virtual void SetCanInteract(bool bIsInteractable) override;
	virtual bool GetCanInteract() override;
	virtual void MoveCombatActor() override;
	virtual void ReceiveDamage(int32 Damage) override;
	virtual float GetCost() override;
	virtual int32 GetHealth() override;
	virtual void Kill() override;
	virtual ECombat GetCombatType() override;
	virtual void SetupActorForDefense() override;

protected:
	// Called when the game starts or when spawned
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
	/// Interact widget component
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UInteractComponent* InteractComponent;

	UPROPERTY(EditAnywhere, Category = "Spell Effects")
	class USoundCue* SpellSound;

	/// <summary>
	/// Niagara system component for showing
	/// the Spell
	/// </summary>
	UPROPERTY(VisibleAnywhere)
	class UNiagaraComponent* SpellEffectComponent;

	/// <summary>
	/// Niagara System effect for Spell destruction
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Spell Effects")
	class UNiagaraSystem* SpellEffect;
#pragma endregion

#pragma region Callbacks
	UFUNCTION()
	void OnSpellClicked(
		UPrimitiveComponent* TouchedComp,
		FKey ButtonPressed
	);
#pragma endregion
};
