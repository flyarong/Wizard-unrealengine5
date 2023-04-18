// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Wizard/WizardTypes/DistrictNames.h"
#include "District.generated.h"

/**
* Actor class for the different districts
* in the village Map
*/
UCLASS()
class WIZARD_API ADistrict : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADistrict();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/// <summary>
	/// Callback function to component overlap
	/// </summary>
	UFUNCTION()
	virtual void OnBoxBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnBoxEndOverlap(
		UPrimitiveComponent* OverlappedComp, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex
	);

private:

	/// <summary>
	/// GameState pointer
	/// </summary>
	class AWizardGameState* WGameState;

	/// <summary>
	/// Trigger box component for setting up
	/// the District
	/// </summary>
	UPROPERTY(EditAnywhere)
	class UBoxComponent* DistrictBox;

	/// <summary>
	/// Niagara system component for anomalies
	/// </summary>
	UPROPERTY(VisibleAnywhere)
	class UNiagaraComponent* AnomalyComponent;

	/// <summary>
	/// Niagara System effect for Anomaly
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Anomaly Effects")
	class UNiagaraSystem* AnomalyEffect;

	/// <summary>
	/// Base Cost of Power to move
	/// to this District
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "District Properties")
	float PowerCost = 10.f;

	/// <summary>
	/// Maximum number of DarkSpells allowed
	/// in the District without a negative consequence
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "District Properties")
	int32 DarkSpellsAllowed = 3;

	/// <summary>
	/// Name of the district
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "District Properties")
	EDistrict DistrictName;

	/// <summary>
	/// Callback function to check the
	/// number of DarkSpells present in the
	/// District after Prepare state
	/// </summary>
	UFUNCTION()
	void CheckDarkSpellsInDistrict();

	/// <summary>
	/// Function to spawn an Anomaly
	/// in the District
	/// </summary>
	void SpawnAnomaly();

public:
	FORCEINLINE EDistrict GetDistrictName() const { return DistrictName; }
	FORCEINLINE float GetCost() const { return PowerCost; }
};
