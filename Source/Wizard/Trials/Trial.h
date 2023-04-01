// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Wizard/Interfaces/WizardCombatActor.h"
#include "Wizard/WizardTypes/CombatTypes.h"
#include "GameFramework/Actor.h"
#include "Trial.generated.h"

/**
* Actor for handling Trials
*/
UCLASS()
class WIZARD_API ATrial : public AActor, public IWizardCombatActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrial();
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;
	virtual float GetCost() override;
	virtual int32 GetHealth() override;
	virtual void Kill() override;
	virtual ECombat GetCombatType() override;
	virtual void ReceiveDamage(int32 Damage) override;

	/// <summary>
	/// Function to setup the Trial against
	/// the Character
	/// </summary>
	/// <param name="Character">Character facing Trial</param>
	void SetupTrial(class APawn* Character);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
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

	UPROPERTY(EditAnywhere, Category = "Trial Effects")
	class USoundCue* TrialSound;

	/// <summary>
	/// Niagara system component for showing
	/// the Trial
	/// </summary>
	UPROPERTY(VisibleAnywhere)
	class UNiagaraComponent* TrialEffectComponent;

	/// <summary>
	/// Niagara System effect for Trial destruction
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Trial Effects")
	class UNiagaraSystem* TrialEffect;

	/// <summary>
	/// Character who is on Trial
	/// </summary>
	UPROPERTY()
	class AWizardCharacter* TrialCharacter;

	/// <summary>
	/// Boolean for whether or not the Character
	/// has any GoodSpells
	/// </summary>
	UPROPERTY()
	bool bCharacterHasGoodSpell = false;
};
