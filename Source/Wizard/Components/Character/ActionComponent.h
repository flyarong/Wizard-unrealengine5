// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Wizard/WizardTypes/DistrictNames.h"
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
	EDistrict CurrentDistrict;

	UFUNCTION()
	void OnRep_CurrentDistrict();
	
	/// <summary>
	/// Server RPC to set the current district on
	/// the server
	/// </summary>
	UFUNCTION(Server, Reliable)
	void ServerCurrentDistrict(EDistrict District);
	
	/// <summary>
	/// The district the character is currently
	/// overlapping
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_CachedDistrict)
	EDistrict CachedDistrict;

	UFUNCTION()
	void OnRep_CachedDistrict();

	/// <summary>
	/// Server RPC to set the currently overlapping district on
	/// the server
	/// </summary>
	UFUNCTION(Server, Reliable)
	void ServerCachedDistrict(EDistrict District);

public:
	FORCEINLINE EDistrict GetCurrentDistrict() const { return CurrentDistrict; }
	void SetCachedDistrict(EDistrict District);
	void SetCurrentDistrict(EDistrict District);
};
