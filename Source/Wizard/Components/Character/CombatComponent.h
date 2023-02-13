// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WIZARD_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatComponent();
	friend class AWizardCharacter;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

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
	/// Material Instance for the Spell Bar
	/// </summary>
	UPROPERTY()
	class UMaterialInstanceDynamic* SpellBar;
};
