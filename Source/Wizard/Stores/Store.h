// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Wizard/Items/Item.h"
#include "Wizard/WizardTypes/StoreType.h"
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

	/// <summary>
	/// Function to show/hide the Interact Widget
	/// </summary>
	/// <param name="bShowInteractWidget">Whether to show or hide the widget</param>
	void ShowInteractWidget(bool bShowInteractWidget);

	/// <summary>
	/// Function to get the store's catalog
	/// </summary>
	/// <returns>Catalog array holding all the Item structs the Store is offering</returns>
	TArray<FItemDataTable> GetStoreCatalog();

	/// <summary>
	/// Function to add a new random
	/// Item to the catalog
	/// </summary>
	void AddItemToCatalog();

	/// <summary>
	/// Function to remove an Item from the catalog
	/// </summary>
	/// <param name="Item">Item to remove</param>
	void RemoveItemFromCatalog(FItemDataTable Item);

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
	/// Type of the store
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Store")
	EStore StoreType;

	/// <summary>
	/// The array of Items a Store
	/// is currently offer
	/// </summary>
	UPROPERTY()
	TArray<FString> CatalogItems;

	/// <summary>
	/// Number of items a Store is
	/// currently offering
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Store")
	int32 NumOfCatalogItems = 5;

	/// <summary>
	/// The array of Items a Store
	/// can offer
	/// </summary>
	UPROPERTY()
	TMap<FString, FItemDataTable> Products;

	/// <summary>
	/// Array containing all the store's product names
	/// </summary>
	UPROPERTY()
	TArray<FString> ProductKeys;

	/// <summary>
	/// Function to create the Catalog
	/// of the Store
	/// </summary>
	void CreateCatalog();

	/// <summary>
	/// Function that gets all the products
	/// from the Datatables
	/// </summary>
	void GetProducts();

	/// <summary>
	/// Function that gets all the product variations
	/// based on Store Type
	/// </summary>
	/// <returns>Datatables holding the products of the Store</returns>
	TArray<class UDataTable*> GetProductTables();

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
	/// Interact widget component
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* InteractComponent;

	/// <summary>
	/// Overhead widget for showing the store's name
	/// </summary>
	UPROPERTY()
	class UOverheadWidget* OverheadWidget;

	/// <summary>
	/// Interact widget for showing the instruction on how
	/// to interact with the Store Catalog
	/// </summary>
	UPROPERTY()
	class UOverheadWidget* InteractWidget;
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

};
