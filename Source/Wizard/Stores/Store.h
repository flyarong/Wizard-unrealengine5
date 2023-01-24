// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Store.generated.h"

UCLASS()
class WIZARD_API AStore : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStore();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
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

	/// <summary>
	/// Name of the store
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Store")
	FString StoreName;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
