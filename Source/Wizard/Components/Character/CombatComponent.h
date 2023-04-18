// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputCoreTypes.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

/*
* Character Component for handling Combat functionality
*/
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
	/// <param name="Target">Actor who is the target of the Combat</param>
	/// <param name="bCharacterAttacking">Whether or not the Character is attacking or defending</param>
	void InitCombat(const TScriptInterface<class IWizardCombatActor>& Target, bool bCharacterAttacking = true);

	/// <summary>
	/// Function to Stop the Combat
	/// </summary>
	void StopCombat();

	/// <summary>
	/// Function to start the Combat
	/// </summary>
	void StartCombat();

	/// <summary>
	/// Function to end the Combat
	/// </summary>
	void EndCombat();

	/// <summary>
	/// Function to start the Combat
	/// using a Dark Spell
	/// </summary>
	void StartDarkSpellCombat();

	/// <summary>
	/// Function to start the Combat
	/// using a Good Spell
	/// </summary>
	void StartGoodSpellCombat();

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
		
#pragma region Pointers
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
	/// Pointer to the gamemode
	/// </summary>
	UPROPERTY()
	class AWizardGameMode* WGameMode;

	/// <summary>
	/// Target Actor of the Combat
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_CombatTarget)
	TScriptInterface<class IWizardCombatActor> CombatTarget;

	UFUNCTION()
	void OnRep_CombatTarget();
#pragma endregion

#pragma region Animation Montages
	UPROPERTY(EditAnywhere, Category = "Animation Montages")
	class UAnimMontage* CombatMontage;

	/// <summary>
	/// Function to play the Combat Animation Montage
	/// </summary>
	void PlayCombatMontage(FName Section);
#pragma endregion

#pragma region Sounds
	UPROPERTY(EditAnywhere, Category = "Combat Sounds")
	class USoundCue* StartSound;

	UPROPERTY(EditAnywhere, Category = "Combat Sounds")
	USoundCue* CastSound;

	UPROPERTY(EditAnywhere, Category = "Combat Sounds")
	USoundCue* CastFailSound;

	UPROPERTY(EditAnywhere, Category = "Combat Sounds")
	USoundCue* SuccessSound;

	UPROPERTY(EditAnywhere, Category = "Combat Sounds")
	USoundCue* FailSound;

	UPROPERTY(EditAnywhere, Category = "Combat Sounds")
	USoundCue* HitSound;
#pragma endregion

#pragma region Niagara
	/// <summary>
	/// Niagara system component for handling
	/// the Combat Effects
	/// </summary>
	UPROPERTY()
	class UNiagaraComponent* CombatEffectComponent;

	/// <summary>
	/// Niagara System effect for Casting
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Combat Effects")
	class UNiagaraSystem* CastEffect;

	/// <summary>
	/// Niagara System effect for Hitting
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Combat Effects")
	UNiagaraSystem* HitEffect;

	/// <summary>
	/// Function to play Niagara effects during combat
	/// </summary>
	/// <param name="Effect">Niagara System to spawn</param>
	/// <param name="Target">Target to spawn the System at</param>
	void PlayNiagaraEffect(UNiagaraSystem* Effect, AActor* Target);
#pragma endregion

	/// <summary>
	/// Timer handle to wait for input during the
	/// current Spell step
	/// </summary>
	FTimerHandle CurrentStepTimer;

	/// <summary>
	/// Rate of success for the Character in Combat
	/// </summary>
	UPROPERTY()
	float SuccessRate = 0.f;

	/// <summary>
	/// Function to set the SuccessRate
	/// at the beginning of Combat
	/// </summary>
	void SetSuccessRate();

	/// <summary>
	/// Number of successes achieved by the Character
	/// during Combat
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_Successes)
	float Successes = 0.f;

	UFUNCTION()
	void OnRep_Successes();

	/// <summary>
	/// Boolean for whether or not the
	/// Character is attacking or defending
	/// </summary>
	UPROPERTY(Replicated)
	bool bIsAttacking = true;

	/// <summary>
	/// Function to setup Combat gameplay
	/// </summary>
	void SetupCombat();

	/// <summary>
	/// Function to reset the gameplay from Combat 
	/// to Default
	/// </summary>
	void Reset();

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
	/// Multicast RPC to reset the SpellBar
	/// </summary>
	UFUNCTION(NetMulticast, UnReliable)
	void MulticastResetSpellBar();

	/// <summary>
	/// Boolean for determining whether or not
	/// the SpellBar should be updated with the proper
	/// Amount
	/// </summary>
	UPROPERTY()
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
	float ProgressTime = .75f;

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
	/// Boolean for whether or not the Current Step
	/// was successful or not
	/// </summary>
	UPROPERTY()
	bool bStepWasSuccessful = false;

	/// <summary>
	/// Function to handle the initiation
	/// of the next step
	/// </summary>
	void InitNextStep();

	/// <summary>
	/// Function to remove the previous
	/// Spell step from the screen
	/// </summary>
	void RemovePreviousStep();

	/// <summary>
	/// Function to calculate the outcome of
	/// the Combat once the Attack ends
	/// </summary>
	void CalculateCombatAttackResult();

	/// <summary>
	/// Function to calculate the outcome of
	/// the Combat once Defending ends
	/// </summary>
	void CalculateCombatDefendResult();

	/// <summary>
	/// Client RPC to trigger a local message
	/// that the Character is out of Power
	/// </summary>
	UFUNCTION(Client, UnReliable)
	void ClientNotEnoughPowerMessage();

	/// <summary>
	/// Multicast RPC to replicate some functionality
	/// on all machines when Combat starts
	/// </summary>
	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartCombat();

	/// <summary>
	/// Multicast RPC to replicate some functionality
	/// on all machines when Spell Step is successful
	/// </summary>
	UFUNCTION(NetMulticast, UnReliable)
	void MulticastStepSuccess();

	/// <summary>
	/// Multicast RPC to replicate some functionality
	/// on all machines when Spell Step fails
	/// </summary>
	UFUNCTION(NetMulticast, UnReliable)
	void MulticastStepFail();

	/// <summary>
	/// Multicast RPC to replicate some functionality
	/// on all machines when applying Hit on Target in Combat
	/// </summary>
	UFUNCTION(NetMulticast, Reliable)
	void MulticastCombatHit();

	/// <summary>
	/// Multicast RPC to replicate some functionality
	/// on all machines when Combat fails
	/// </summary>
	UFUNCTION(NetMulticast, Reliable)
	void MulticastCombatFail();

public:
	FORCEINLINE float GetIsAttacking() const { return bIsAttacking; }
	FORCEINLINE AActor* GetCombatTarget() const { return Cast<AActor>(CombatTarget.GetObject()); }
};
