// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PointOfInterestComponent.generated.h"

/**
* Component class to handle Point of Interest functionality
* on MiniMap for any Actor
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WIZARD_API UPointOfInterestComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPointOfInterestComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	/// <summary>
	/// Function to setup the Point Of Interest
	/// Should be called from owning Actor
	/// </summary>
	void SetupPOI();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	/// <summary>
	/// Whether or not to clamp the POI icon
	/// to the edge of the MiniMap, or make it
	/// disappear once the POI is far enough
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Point Of Interest")
	bool bIsStatic = false;

	/// <summary>
	/// The image that this POI is going to use
	/// on the MiniMap
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Point Of Interest")
	class UTexture2D* IconImage;
	
public:
	FORCEINLINE bool GetIsStatic() const { return bIsStatic; }
	FORCEINLINE UTexture2D* GetIconImage() const { return IconImage; }
};
