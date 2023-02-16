// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Wizard/Items/Item.h"
#include "Wizard/WizardTypes/DistrictNames.h"
#include "WizardCharacter.generated.h"

UCLASS(Blueprintable)
class AWizardCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AWizardCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;

	/// <summary>
	/// Function for initializing the character's properties
	/// </summary>
	/// <param name="PlayerName">Player's name</param>
	/// <param name="RowName">Selected character's name</param>
	void InitGameplayCharacter(FString PlayerName, FName RowName);

	/// <summary>
	/// Function to add a new Item to the
	/// Character's Items
	/// </summary>
	/// <param name="ItemIndex">Items index</param>
	/// <param name="ItemRow">Item DataTable row element</param>
	void AddNewItem(int32 ItemIndex, FItemDataTable ItemRow);

	/// <summary>
	/// Server RPC to use an Item from
	/// the Character's Items
	/// </summary>
	/// <param name="ItemIndex">Item's index the Character wants to use</param>
	UFUNCTION(Server, Reliable)
	void ServerUseItem(int32 ItemIndex);

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

#pragma region Items
	/// <summary>
	/// Replicated array used to control
	/// the Character's Items from the server
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_ItemIndexes)
	TArray<int32> ItemIndexes;

	UFUNCTION()
	void OnRep_ItemIndexes();

	/// <summary>
	/// Last Item that has been added
	/// to the Character's Items
	/// </summary>
	UPROPERTY(Replicated)
	FItemDataTable LatestItem;

	/// <summary>
	/// Map that contains the Character's Items
	/// on the server
	/// </summary>
	UPROPERTY()
	TMap<int32, FItemDataTable> Items;

	/// <summary>
	/// Function to add the new Item
	/// to the HUD
	/// </summary>
	void AddHUDItem(int32 ItemIndex);
#pragma endregion

public:
	FORCEINLINE UActionComponent* GetAction() const { return Action; }
	FORCEINLINE UAttributeComponent* GetAttribute() const { return Attribute; }
	FORCEINLINE UCombatComponent* GetCombat() const { return Combat; }
	FORCEINLINE UCharacterPOIComponent* GetPOI() const { return POI; }
	FORCEINLINE AWizardPlayerController* GetWizardController() { return PlayerController; }
	FORCEINLINE FItemDataTable GetLatestItem() const { return LatestItem; }
};

