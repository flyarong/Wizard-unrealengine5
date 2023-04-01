// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Wizard/WizardTypes/BoostTypes.h"
#include "Item.generated.h"

/**
* Data structure for Items
*/
USTRUCT(BlueprintType)
struct FItemDataTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	/// <summary>
	/// Item's name
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemName;

	/// <summary>
	/// Item's image
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* ItemImage;

	/// <summary>
	/// Item's cost
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Cost;

	/// <summary>
	/// Skill's image
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* BoostImage;

	/// <summary>
	/// What skill does it boost
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBoost BoostType;

	/// <summary>
	/// Item's boost amount
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BoostAmount;

	/// <summary>
	/// Custom operator for Equals
	/// </summary>
	/// <param name="Rhs">Right hand side Item to compare to</param>
	/// <returns>Whether the two Items are the same one</returns>
	bool operator==(const FItemDataTable& Rhs) const;

	/// <summary>
	/// Custom GetTypeHash to be able to use this struct
	/// as key in TMap
	/// </summary>
	friend uint32 GetTypeHash(const FItemDataTable& ItemRow);
};

UCLASS()
class WIZARD_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	/// <summary>
	/// Function to assign an FItemDataTable Row
	/// to this Item Actor
	/// </summary>
	/// <returns>The randomly selected ItemRow</returns>
	FItemDataTable GetItemRow();

	/// <summary>
	/// DataTable object based on this
	/// Item's Boost type
	/// </summary>
	UPROPERTY(EditAnywhere)
	UDataTable* ItemTableObject;

	/// <summary>
	/// FItemDataTable Row based on this Item's
	/// Boost type
	/// </summary>
	UPROPERTY()
	FItemDataTable ItemRow;

private:

	UPROPERTY(EditAnywhere)
	class USphereComponent* OverlapSphere;

	UPROPERTY(EditAnywhere)
	class USoundCue* PickupSound;

	UPROPERTY(VisibleAnywhere)
	class UNiagaraComponent* ItemEffectComponent;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* PickupEffect;

};
