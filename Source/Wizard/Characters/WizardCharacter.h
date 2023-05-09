// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Wizard/Interfaces/PublicMessageActor.h"
#include "Wizard/Items/Item.h"
#include "Wizard/WizardTypes/DistrictNames.h"
#include "WizardCharacter.generated.h"

/// <summary>
/// Delegate type for leaving the game
/// </summary>
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLeftGame);

UCLASS(Blueprintable)
class AWizardCharacter : public ACharacter, public IPublicMessageActor
{
	GENERATED_BODY()

public:
	AWizardCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;
	virtual class UTexture2D* GetIcon() override;

	/// <summary>
	/// Function for initializing the character's properties
	/// </summary>
	/// <param name="PlayerName">Player's name</param>
	/// <param name="RowName">Selected character's name</param>
	void InitGameplayCharacter(FString PlayerName, FName RowName);

	/// <summary>
	/// Delegate to broadcast when the player
	/// is leaving the game
	/// </summary>
	FOnLeftGame OnLeftGameDelegate;

	/// <summary>
	/// Function to leave the game
	/// </summary>
	void LeaveGame();

	/// <summary>
	/// Function that refills some amount of Power
	/// to the Character at the start of each turn
	/// </summary>
	void RefillPower();

	/// <summary>
	/// Function to add a new Item to the
	/// Character's Items
	/// </summary>
	/// <param name="ItemRow">Item DataTable row element</param>
	void AddNewItem(const FItemDataTable& ItemRow);

	/// <summary>
	/// Server RPC to use an Item from
	/// the Character's Items
	/// </summary>
	/// <param name="Item">Item DataTable Row the Character wants to use</param>
	UFUNCTION(Server, Reliable)
	void ServerUseItem(const FItemDataTable& Item);

	/// <summary>
	/// Server RPC to equip an outfit on the Character
	/// </summary>
	/// <param name="Equipment">Equipment DataTable Row the Character wants to equip</param>
	UFUNCTION(Server, Reliable)
	void ServerEquipOutfit(const FItemDataTable& Equipment);

	/// <summary>
	/// Server RPC to unequip an outfit on the Character
	/// </summary>
	/// <param name="Equipment">Equipment DataTable Row the Character wants to unequip</param>
	UFUNCTION(Server, Reliable)
	void ServerUnEquipOutfit(const FItemDataTable& Equipment);

	/// <summary>
	/// Multicast RPC to play the Interact Animation Montage
	/// </summary>
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayInteractMontage(FName Section);

	/// <summary>
	/// Function to Play an AnimMontage
	/// on the Character
	/// </summary>
	/// <param name="Montage">Montage to play</param>
	/// <param name="Section">Section to jump to</param>
	void PlayMontage(UAnimMontage* Montage, FName Section);

	/// <summary>
	/// Callback function to when an Animation Montage
	/// Notify begins
	/// </summary>
	/// <param name="NotifyName">Name of the AnimNotify</param>
	UFUNCTION()
	void OnMontageNotifyBegin(FName NotifyName, const struct FBranchingPointNotifyPayload& BranchingPointPayload);

	/// <summary>
	/// Function to play a SoundCue
	/// </summary>
	/// <param name="Sound">Sound to play</param>
	void PlaySound(class USoundCue* Sound);

	/// <summary>
	/// Server RPC to interrupt the Character's movement
	/// </summary>
	UFUNCTION(Server, Reliable)
	void ServerInterruptMovement();

protected:
	virtual void BeginPlay() override;

private:

	/// <summary>
	/// Player controller pointer
	/// </summary>
	UPROPERTY()
	class AWizardPlayerController* PlayerController;

#pragma region CharacterComponents
	/// <summary>
	/// Mesh for holding the Magic Staff
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MagicStaff;

	/// <summary>
	/// Overhead widget for showing the player's name
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* OverheadWidget;

	/// <summary>
	/// Character's Attribute component: handling
	/// all the functionality regarding character
	/// attributes (e.g. health)
	/// </summary>
	UPROPERTY(VisibleAnywhere)
	class UAttributeComponent* Attribute;

	/// <summary>
	/// Character's Action component: handling
	/// all the actions a character can do
	/// </summary>
	UPROPERTY(VisibleAnywhere)
	class UActionComponent* Action;

	/// <summary>
	/// Character's Combat component: handling
	/// all the combat-related functionality
	/// </summary>
	UPROPERTY(VisibleAnywhere)
	class UCombatComponent* Combat;

	/// <summary>
	/// Mesh for the Spell Bar
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* CombatMesh;

	/// <summary>
	/// Static mesh for the CombatMesh Component
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Spell Bar")
	class UStaticMesh* CombatSM;

	/// <summary>
	/// Material for the CombatMesh Component
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Spell Bar")
	class UMaterial* CombatMaterial;

	/// <summary>
	/// Character's Point of Interest component:
	/// shows the location of the Character on the MiniMap
	/// </summary>
	UPROPERTY(VisibleAnywhere)
	class UCharacterPOIComponent* POI;
#pragma endregion

#pragma region Animation
	UPROPERTY(EditAnywhere, Category = "Animation Montages")
	class UAnimMontage* InteractMontage;

	/// <summary>
	/// Boolean for whether or not the Character
	/// is in Combat
	/// </summary>
	UPROPERTY()
	bool bIsInCombat = false;

	/// <summary>
	/// Boolean for whether or not the Character
	/// is attacking in Combat
	/// </summary>
	UPROPERTY()
	bool bIsAttacking = true;
#pragma endregion

#pragma region Items
	/// <summary>
	/// Replicated array used to control
	/// the Character's Items from the server
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_Items)
	TArray<FItemDataTable> Items;

	UFUNCTION()
	void OnRep_Items();

	/// <summary>
	/// Function to update the Character's Inventory
	/// </summary>
	void UpdateInventory();

	/// <summary>
	/// Function to boost an Attribute
	/// </summary>
	/// <param name="Item">Item used to boost the Attribute</param>
	void BoostAttribute(const FItemDataTable& Item);

	/// <summary>
	/// Function to decrease an Attribute
	/// </summary>
	/// <param name="Item">Item used to decrease the Attribute</param>
	void DecreaseAttribute(const FItemDataTable& Item);
#pragma endregion

#pragma region Equipments
	/// <summary>
	/// Replicated array used to control
	/// the Character's Equipments from the server
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_Equipments)
	TArray<FItemDataTable> Equipments;

	UFUNCTION()
	void OnRep_Equipments();

	/// <summary>
	/// Function to update the Character's Equipments
	/// </summary>
	void UpdateEquipments();

	/// <summary>
	/// Replicated array used to control
	/// the Character's Outfit from the server
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_Outfit)
	TArray<FItemDataTable> Outfit;

	UFUNCTION()
	void OnRep_Outfit();

	/// <summary>
	/// Function to update the Character's Outfit
	/// </summary>
	void UpdateOutfit();

	/// <summary>
	/// Function that checks whether or not the Character
	/// has an outfit type already equipped
	/// </summary>
	/// <param name="OutfitType">Type of outfit to check for</param>
	/// <returns>Whether or not the Character has an outfit equipped with that type</returns>
	bool HasOutfitTypeOn(EOutfit OutfitType);
#pragma endregion

#pragma region Sounds
	UPROPERTY(EditAnywhere, Category = "Item Sounds")
	class USoundCue* OpenInventorySound;

	UPROPERTY(EditAnywhere, Category = "Item Sounds")
	USoundCue* UseSound;

	UPROPERTY(EditAnywhere, Category = "Equipment Sounds")
	USoundCue* OpenEquipmentSound;

	UPROPERTY(EditAnywhere, Category = "Equipment Sounds")
	USoundCue* EquipSound;
#pragma endregion

	/// <summary>
	/// Server RPC for leaving the game
	/// </summary>
	UFUNCTION(Server, Reliable)
	void ServerLeaveGame();

	/// <summary>
	/// Multicast RPC to broadcast Character Movement
	/// </summary>
	UFUNCTION(NetMulticast, Reliable)
	void MulticastInterruptMovement();

public:
	FORCEINLINE UActionComponent* GetAction() const { return Action; }
	FORCEINLINE UAttributeComponent* GetAttribute() const { return Attribute; }
	FORCEINLINE UCombatComponent* GetCombat() const { return Combat; }
	FORCEINLINE UCharacterPOIComponent* GetPOI() const { return POI; }
	FORCEINLINE AWizardPlayerController* GetWizardController() { return PlayerController; }
	FORCEINLINE TArray<FItemDataTable> GetItems() const { return Items; }
	FORCEINLINE TArray<FItemDataTable> GetEquipments() const { return Equipments; }
	FORCEINLINE bool HasEquipment(const FItemDataTable& Equipment) const { return Equipments.Contains(Equipment) || Outfit.Contains(Equipment); }
	FORCEINLINE bool GetIsInCombat() const { return bIsInCombat; }
	FORCEINLINE void SetIsInCombat(bool bInCombat) { bIsInCombat = bInCombat; }
	FORCEINLINE bool GetIsAttacking() const { return bIsAttacking; }
	FORCEINLINE void SetIsAttacking(bool bAttacking) { bIsAttacking = bAttacking; }
	FORCEINLINE void PlayUseSound() { PlaySound(UseSound); }
	FORCEINLINE void PlayEquipSound() { PlaySound(EquipSound); }
	FORCEINLINE void PlayOpenInventorySound() { PlaySound(OpenInventorySound); }
	FORCEINLINE void PlayOpenEquipmentSound() { PlaySound(OpenEquipmentSound); }
};

