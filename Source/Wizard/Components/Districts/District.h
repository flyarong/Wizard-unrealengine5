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

private:
	UPROPERTY(EditAnywhere)
	class UBoxComponent* DistrictBox;

	UPROPERTY(EditAnywhere)
	float EnergyCost = 10.f;

	/// <summary>
	/// Name of the district
	/// </summary>
	UPROPERTY(EditAnywhere)
	EDistrict DistrictName;

public:
	FORCEINLINE EDistrict GetDistrictName() const { return DistrictName; }
	FORCEINLINE float GetCost() const { return EnergyCost; }
};
