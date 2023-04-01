// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Wizard/Items/Item.h"
#include "Wizard/WizardTypes/AttributeTypes.h"
#include "ActionComponent.generated.h"

/// <summary>
/// Delegate for broadcasting when the
/// Defense Combat has ended for the Character
/// </summary>
/// <param name="AttackedCharacter">The Character</param>
/// <param name="NewAttacker">Next opponent to defend against</param>
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnDefenseCombatEndedDelegate, class AWizardCharacter*, class AActor*);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WIZARD_API UActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActionComponent();
	friend class AWizardCharacter;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/// <summary>
	/// Function to set the WizardActor the
	/// Player is currently interacting with
	/// </summary>
	void SetCurrentWizardActor(const TScriptInterface<class IWizardActor>& WizardActor);
	
	/// <summary>
	/// Function to leave the currently interacted
	/// WizardActor
	/// </summary>
	void LeaveWizardActor();

	/// <summary>
	/// Function to open the current store's
	/// catalog
	/// </summary>
	void OpenCatalog();

	/// <summary>
	/// Function to close the current store's
	/// catalog
	/// </summary>
	void CloseCatalog();

	/// <summary>
	/// Function to buy an item from
	/// the current store's catalog
	/// </summary>
	/// <param name="ItemRow">Item to buy</param>
	UFUNCTION(Server, Reliable)
	void ServerBuyItem(const FItemDataTable& ItemRow);
	
	/// <summary>
	/// Server RPC to initiate Attack Combat against
	/// a WizardCombatActor
	/// </summary>
	UFUNCTION(Server, Reliable)
	void ServerInitAttackCombat();

	/// <summary>
	/// Function that adds a new Attacker
	/// Combat Actor to the list of Attackers
	/// </summary>
	/// <param name="NewAttacker">New Attacker who wants to Attack the Character</param>
	void SetAttacker(AActor* NewAttacker);

	/// <summary>
	/// Function to initialize Defense
	/// Combat
	/// </summary>
	/// <param name="Attacker">Attacking Actor</param>
	void InitDefenseCombat(AActor* Attacker);

	/// <summary>
	/// Function to initialize a Trial
	/// Combat
	/// </summary>
	/// <param name="Trial">Trial to face</param>
	void InitTrialCombat(class ATrial* Trial);

	/// <summary>
	/// Server RPC to cancel the Combat
	/// </summary>
	UFUNCTION(Server, Reliable)
	void ServerCancelCombat();

	/// <summary>
	/// Server RPC to start the Combat
	/// </summary>
	UFUNCTION(Server, Reliable)
	void ServerStartCombat();

	/// <summary>
	/// Server RPC to start the Combat
	/// using a Dark Spell
	/// </summary>
	UFUNCTION(Server, Reliable)
	void ServerStartDarkSpellCombat();

	/// <summary>
	/// Server RPC to start the Combat
	/// using a Good Spell
	/// </summary>
	UFUNCTION(Server, Reliable)
	void ServerStartGoodSpellCombat();

	/// <summary>
	/// Server RPC to validate the Combat Input
	/// </summary>
	/// <param name="Input">Input value from Controller</param>
	UFUNCTION(Server, Reliable)
	void ServerValidateCombatInput(int32 Input);

	/// <summary>
	/// Function to end the Attack Combat
	/// </summary>
	void EndAttack();

	/// <summary>
	/// Function to end the Defense Combat
	/// </summary>
	void EndDefense();

	/// <summary>
	/// Delegate to broadcast when the Character
	/// has ended Defense Combat
	/// </summary>
	FOnDefenseCombatEndedDelegate OnDefenseCombatEndedDelegate;

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
	/// Player controller pointer
	/// </summary>
	UPROPERTY()
	class AWizardPlayerController* Controller;

	/// <summary>
	/// Player state pointer
	/// </summary>
	UPROPERTY()
	class AWizardPlayerState* PlayerState;
#pragma endregion

#pragma region Movement
	/// <summary>
	/// District the player is currently at
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_CurrentDistrict)
	class ADistrict* CurrentDistrict;

	UFUNCTION()
	void OnRep_CurrentDistrict();

	/// <summary>
	/// Function to update the current district
	/// on the HUD
	/// </summary>
	void UpdateHUDCurrentDistrict();
#pragma endregion

#pragma region Interaction
	/// <summary>
	/// Actor the Character can interact with
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_OverlappedWizardActor)
	TScriptInterface<class IWizardActor> OverlappedWizardActor;

	UFUNCTION()
	void OnRep_OverlappedWizardActor(const TScriptInterface<IWizardActor>& PreviousWizardActor);
#pragma endregion

#pragma region Buying
	/// <summary>
	/// Whether the player can browse the
	/// store's catalog or not
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_CanBrowse)
	bool bCanBrowse = false;

	UFUNCTION()
	void OnRep_CanBrowse();

	/// <summary>
	/// Function to show the currently
	/// browsed store's catalog
	/// </summary>
	void ShowStoreCatalog();

	UPROPERTY(EditAnywhere, Category = "Store Sounds")
	class USoundCue* SuccessfulPurchaseSound;

	UPROPERTY(EditAnywhere, Category = "Store Sounds")
	USoundCue* FailedPurchaseSound;

	/// <summary>
	/// Client RPC to play a SoundCue
	/// </summary>
	UFUNCTION(Client, Reliable)
	void ClientPlaySound(USoundCue* Sound);
#pragma endregion

#pragma region Messaging
	/// <summary>
	/// Client RPC to add a Local Message to the HUD
	/// </summary>
	/// <param name="Message">The Message</param>
	/// <param name="AttributeType">The Attribute the message refers to</param>
	UFUNCTION(Client, UnReliable)
	void ClientAddLocalMessage(const FString& Message, EAttribute AttributeType);
#pragma endregion

#pragma region Combat	
	/// <summary>
	/// Boolean for whether or not the Character is in Combat
	/// </summary>
	UPROPERTY()
	bool bInCombat = false;

	/// <summary>
	/// Array of Combat Actors currently overlapping
	/// the Character to attack
	/// </summary>
	UPROPERTY()
	TArray<AActor*> Attackers = TArray<AActor*>();

	/// <summary>
	/// Multicast RPC to make the Character
	/// face the Target Combat Actor
	/// </summary>
	/// <param name="Target">Actor the Character is in Combat with</param>
	UFUNCTION(NetMulticast, Reliable)
	void MulticastAimCharacterToTarget(class AActor* Target);
#pragma endregion

public:
	FORCEINLINE ADistrict* GetCurrentDistrict() const { return CurrentDistrict; }
	void SetCurrentDistrict(ADistrict* District);
	FORCEINLINE const TScriptInterface<IWizardActor>& GetOverlappedWizardActor() const { return OverlappedWizardActor; }
	FORCEINLINE bool GetIsInCombat() const { return bInCombat; }
};
