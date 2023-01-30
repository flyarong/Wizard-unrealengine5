// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Wizard/Items/Item.h"
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
	void BuyItem(FItemDataTable ItemRow);

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
	UPROPERTY(Replicated)
	class AStore* CurrentStore;

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
#pragma endregion

public:
	FORCEINLINE ADistrict* GetCurrentDistrict() const { return CurrentDistrict; }
	void SetCurrentDistrict(ADistrict* District);
	FORCEINLINE AStore* GetCurrentStore() const { return CurrentStore; }
	FORCEINLINE void SetCurrentStore(AStore* Store) { CurrentStore = Store; };

};
