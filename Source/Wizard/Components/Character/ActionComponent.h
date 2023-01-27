// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

	void SetCurrentStore(class AStore* Store);

	void LeaveStore();

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
	/// Player controller pointer
	/// </summary>
	UPROPERTY()
	class AWizardPlayerController* Controller;

	/// <summary>
	/// Player state pointer
	/// </summary>
	UPROPERTY()
	class AWizardPlayerState* PlayerState;

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

	/// <summary>
	/// Store the player is currently at
	/// </summary>
	UPROPERTY(Replicated)
	AStore* CachedStore;

	/// <summary>
	/// Store the player is currently
	/// browsing
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_CurrentStore)
	AStore* CurrentStore;

	UFUNCTION()
	void OnRep_CurrentStore();

	/// <summary>
	/// Function to show the currently
	/// browsed store's catalog
	/// </summary>
	void ShowStoreCatalog();

public:
	FORCEINLINE ADistrict* GetCurrentDistrict() const { return CurrentDistrict; }
	void SetCurrentDistrict(ADistrict* District);
	FORCEINLINE AStore* GetCurrentStore() const { return CurrentStore; }
	FORCEINLINE AStore* GetCachedStore() const { return CachedStore; }
	FORCEINLINE void SetCachedStore(AStore* Store) { if (Store) CachedStore = Store; }
};
