// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Wizard/Items/Item.h"
#include "Wizard/WizardTypes/AttributeTypes.h"
#include "ActionComponent.generated.h"


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
	/// Function to set the Store the
	/// Player is currently at
	/// </summary>
	void SetCurrentStore(AStore* Store);

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
	/// Function to leave the currently visited
	/// store
	/// </summary>
	void LeaveStore();

	/// <summary>
	/// Function to buy an item from
	/// the current store's catalog
	/// </summary>
	/// <param name="ItemRow">Item to buy</param>
	UFUNCTION(Server, Reliable)
	void ServerBuyItem(const FItemDataTable& ItemRow);

	/// <summary>
	/// Function to set the currently overlapped Combat Actor
	/// </summary>
	/// <param name="CombatActor">Combat Actor being overlapped</param>
	void SetOverlappedCombatActor(class AWizardCombatActor* CombatActor);

	/// <summary>
	/// Function to execute when overlapping with a Combat Actor
	/// ends
	/// </summary>
	void LeaveCombatActor();
	
	/// <summary>
	/// Server RPC to initiate Combat against
	/// a Spell
	/// </summary>
	UFUNCTION(Server, Reliable)
	void ServerInitSpellCombat();

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
	/// Server RPC to validate the Combat Input
	/// </summary>
	/// <param name="Input">Input value from Controller</param>
	UFUNCTION(Server, Reliable)
	void ServerValidateCombatInput(int32 Input);

	/// <summary>
	/// Function to end the Combat
	/// </summary>
	void EndCombat();

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

#pragma region Buying
	/// <summary>
	/// Store the player is currently
	/// browsing
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_CurrentStore)
	class AStore* CurrentStore;

	UFUNCTION()
	void OnRep_CurrentStore(AStore* PreviousStore);

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
	/// Combat Actor the Character is currently overlapping
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_OverlappedCombatActor)
	AWizardCombatActor* OverlappedCombatActor;

	UFUNCTION()
	void OnRep_OverlappedCombatActor(AWizardCombatActor* PreviousActor);
		
	/// <summary>
	/// Multicast RPC to make the Character
	/// face the Target Combat Actor
	/// </summary>
	/// <param name="Target">Actor the Character is in Combat with</param>
	UFUNCTION(NetMulticast, Reliable)
	void MulticastAimCharacterToTarget(class AWizardActor* Target);
#pragma endregion

public:
	FORCEINLINE ADistrict* GetCurrentDistrict() const { return CurrentDistrict; }
	void SetCurrentDistrict(ADistrict* District);
	FORCEINLINE AStore* GetCurrentStore() const { return CurrentStore; }
	FORCEINLINE AWizardCombatActor* GetOverlappedCombatActor() const { return OverlappedCombatActor; }

};
