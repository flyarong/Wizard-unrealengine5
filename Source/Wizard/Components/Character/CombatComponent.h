// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputCoreTypes.h"
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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/// <summary>
	/// Function to initialize Combat
	/// </summary>
	/// <param name="AttributeForCombat">Attribute Value to use during Combat</param>
	void InitCombat(int32 AttributeForCombat);

	/// <summary>
	/// Function to Stop the Combat
	/// </summary>
	void StopCombat();

	/// <summary>
	/// Function to start the Combat
	/// </summary>
	void StartCombat();

	/// <summary>
	/// Function to start the Next Spell Step
	/// </summary>
	void StartNextStep();

	/// <summary>
	/// Function to stop the timer of
	/// the current Spell Step
	/// </summary>
	void StopCurrentTimer();

	/// <summary>
	/// Function to validate the Combat Input
	/// </summary>
	/// <param name="Input">Input coming from Controller which contains the SpellInput Index</param>
	void ValidateInput(int32 Input);

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
	/// Pointer to the player controller
	/// </summary>
	UPROPERTY()
	class AWizardPlayerController* WController;

	/// <summary>
	/// Timer handle to wait for input during the
	/// current Spell step
	/// </summary>
	FTimerHandle CurrentStepTimer;

	/// <summary>
	/// The value of the Attribute used
	/// during Combat
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_CombatAttribute)
	int32 CombatAttribute = 0;

	UFUNCTION()
	void OnRep_CombatAttribute();

	/// <summary>
	/// Function to setup Combat on the screen
	/// </summary>
	void SetupCombatHUD();

	/// <summary>
	/// Function to reset the screen from Combat 
	/// to Default
	/// </summary>
	void ResetHUD();

	/// <summary>
	/// Material Instance for the Spell Bar
	/// </summary>
	UPROPERTY()
	class UMaterialInstanceDynamic* SpellBar;

	/// <summary>
	/// The Scalar Parameter Value to be chaned
	/// in the SpellBar
	/// </summary>
	UPROPERTY()
	FName SpellBarParameterValue = FName("Fullness");

	/// <summary>
	/// Function to trigger when the timer
	/// between Spell Steps finished
	/// Sets the current Spell Step
	/// </summary>
	UFUNCTION()
	void SetCurrentSpellStep();

	/// <summary>
	/// Function to setup the Steps for the Spell
	/// </summary>
	void SetSpellSteps();

	/// <summary>
	/// Function to create the SpellIndexes
	/// </summary>
	void SetSpellIndexes();

	/// <summary>
	/// Function to update the SpellBar
	/// </summary>
	/// <param name="DeltaTime">DeltaTime from Tick</param>
	void UpdateSpellBar(float DeltaTime);

	/// <summary>
	/// Boolean for determining whether or not
	/// the combat has started
	/// </summary>
	UPROPERTY(Replicated)
	bool bSpellBarShouldUpdate = false;

	/// <summary>
	/// Array containing the key inputs used
	/// in the Spell cast
	/// </summary>
	UPROPERTY(Replicated, EditAnywhere, Category = "Spells")
	TArray<FKey> SpellInputs;

	/// <summary>
	/// Array containing indexes of Step Symbols
	/// </summary>
	UPROPERTY(Replicated)
	TArray<int32> SpellIndexes;

	/// <summary>
	/// Variable to determine how fast
	/// the SpellBar fill
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Spells")
	float ProgressTime = 1.f;

	/// <summary>
	/// Number of Steps required to cast
	/// a Spell
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Spells")
	int32 NumberOfSteps = 6;

	/// <summary>
	/// Number of time allowed for a Spell cast (in seconds)
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Spells")
	float StepTime = 1.5f;

	/// <summary>
	/// Variable to determine how much
	/// amount is to be filled in the SpellBar
	/// </summary>
	UPROPERTY()
	float Amount = 1.f / NumberOfSteps;

	/// <summary>
	/// The rate of fill
	/// </summary>
	UPROPERTY()
	float Rate = Amount / ProgressTime;

	/// <summary>
	/// Amount to fill per frame
	/// </summary>
	UPROPERTY()
	float ValueThisFrame = 0.f;

	/// <summary>
	/// Array containing the Spell Index corresponding
	/// to the key inputs required for each step
	/// </summary>
	UPROPERTY(Replicated)
	TArray<int32> Steps;

	/// <summary>
	/// Index of the current Step
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_StepIndex)
	int32 StepIndex = -1;

	UFUNCTION()
	void OnRep_StepIndex();

	/// <summary>
	/// Function to add the Current step
	/// to the screen
	/// </summary>
	void AddCurrentStep();

	/// <summary>
	/// Boolean for whether or not to
	/// do the preparations for the next Spell Step
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_InitNextStep)
	bool bInitNextStep = false;

	UFUNCTION()
	void OnRep_InitNextStep();

	/// <summary>
	/// Function to remove the previous
	/// Spell step from the screen
	/// </summary>
	void RemovePreviousStep();
};
