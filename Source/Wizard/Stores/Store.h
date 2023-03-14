// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Wizard/Interfaces/WizardActor.h"
#include "Wizard/Items/Item.h"
#include "Wizard/WizardTypes/StoreTypes.h"
#include "Store.generated.h"

UCLASS()
class WIZARD_API AStore : public AActor, public IWizardActor
{
	GENERATED_BODY()
	
public:	
	AStore();
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void ShowInteractWidget(bool bShowInteractWidget) override;
	virtual class UTexture2D* GetIcon() override;

	/// <summary>
	/// Function to add a new random
	/// Item to the catalog
	/// </summary>
	void AddItemToCatalog();

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
	/// is currently offering
	/// </summary>
	UPROPERTY(Replicated)
	TArray<FItemDataTable> Catalog;

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
	TArray<FItemDataTable> Products;

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
	/// Actor's Point of Interest component:
	/// shows the location of the Actor on the MiniMap
	/// </summary>
	UPROPERTY(VisibleAnywhere)
	class UPointOfInterestComponent* POI;

	/// <summary>
	/// AreaSphere to interact with the Actor
	/// </summary>
	UPROPERTY(VisibleAnywhere)
	class USphereComponent* AreaSphere;

	/// <summary>
	/// Interact widget component
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UInteractComponent* InteractComponent;

	/// <summary>
	/// Mesh of the Store
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StoreMesh;

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
