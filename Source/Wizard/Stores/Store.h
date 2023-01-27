// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Wizard/Items/Item.h"
#include "Store.generated.h"

UCLASS()
class WIZARD_API AStore : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStore();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	/// <summary>
	/// Name of the store
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Store")
	FString StoreName;

	/// <summary>
	/// The array of Items a Store
	/// can offer
	/// </summary>
	UPROPERTY()
	TArray<FItemDataTable> Catalog;

	/// <summary>
	/// Function to create the Catalog
	/// of the Store
	/// </summary>
	void CreateCatalog();

#pragma region Components
	/// <summary>
	/// Store's Point of Interest component:
	/// shows the location of the Store on the MiniMap
	/// </summary>
	UPROPERTY(VisibleAnywhere)
	class UPointOfInterestComponent* POI;

	/// <summary>
	/// Mesh of the Store
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StoreMesh;

	/// <summary>
	/// AreaSphere to interact with the Store
	/// </summary>
	UPROPERTY(VisibleAnywhere)
	class USphereComponent* AreaSphere;

	/// <summary>
	/// Overhead widget component
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* OverheadComponent;

	/// <summary>
	/// Overhead widget for showing the store's name
	/// </summary>
	UPROPERTY()
	class UOverheadWidget* OverheadWidget;
#pragma endregion

#pragma region Callbacks
	UFUNCTION()
	void OnStoreBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnStoreEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	UFUNCTION()
	void OnStoreClicked(
		UPrimitiveComponent* TouchedComp,
		FKey ButtonPressed
	);
#pragma endregion	

public:
	FORCEINLINE TArray<FItemDataTable> GetStoreCatalog() const { return Catalog; }
};
