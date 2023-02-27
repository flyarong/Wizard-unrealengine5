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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/// <summary>
	/// Item's name
	/// </summary>
	UPROPERTY()
	FString ItemName;

	/// <summary>
	/// Item's image
	/// </summary>
	UPROPERTY()
	UTexture2D* ItemImage;

	/// <summary>
	/// Item's cost
	/// </summary>
	UPROPERTY()
	int32 Cost;

	/// <summary>
	/// Skill's image
	/// </summary>
	UPROPERTY()
	UTexture2D* BoostImage;

	/// <summary>
	/// What skill does it boost
	/// </summary>
	UPROPERTY()
	EBoost BoostType;

	/// <summary>
	/// Item's boost amount
	/// </summary>
	UPROPERTY()
	int32 BoostAmount;

public:	
	FORCEINLINE FString GetItemName() const { return ItemName; }
	FORCEINLINE UTexture2D* GetItemImage() const { return ItemImage; }
	FORCEINLINE int32 GetCost() const { return Cost; }
	FORCEINLINE UTexture2D* GetBoostImage() const { return BoostImage; }
	FORCEINLINE EBoost GetBoostType() const { return BoostType; }
	FORCEINLINE int32 GetBoostAmount() const { return BoostAmount; }
};
