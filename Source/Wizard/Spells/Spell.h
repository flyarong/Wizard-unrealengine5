// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Wizard/Actors/WizardActor.h"
#include "Spell.generated.h"

/**
* Spell Actor base class
*/
UCLASS()
class WIZARD_API ASpell : public AWizardActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpell();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/// <summary>
	/// Function to set the result of
	/// destroying the Spell
	/// TODO - need to decide function name later
	/// </summary>
	virtual void SetSpellScore();

private:
#pragma region Components
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
	void OnSpellBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnSpellEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	UFUNCTION()
	void OnSpellClicked(
		UPrimitiveComponent* TouchedComp,
		FKey ButtonPressed
	);
#pragma endregion	
};
