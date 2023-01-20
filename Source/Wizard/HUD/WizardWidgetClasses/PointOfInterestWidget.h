// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PointOfInterestWidget.generated.h"

/**
 * Widget class for showing a Point Of Interest on the
 * MiniMap
 */
UCLASS()
class WIZARD_API UPointOfInterestWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	/// <summary>
	/// Pointer to the MiniMap widget
	/// </summary>
	UPROPERTY()
	class UMiniMapWidget* MiniMap;

	/// <summary>
	/// Whether or not to clamp the POI icon
	/// to the edge of the MiniMap (true) or
	/// hide it (false)
	/// </summary>
	UPROPERTY()
	bool bIsStatic = false;

	/// <summary>
	/// Which Actor in the scene this
	/// icon belongs to
	/// </summary>
	UPROPERTY()
	class AActor* Owner;

	/// <summary>
	/// POI's custom image
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	class UImage* POIImage;

	/// <summary>
	/// Default image to show if no
	/// custom is provided
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	class UThrobber* DefaultImage;

private:

	/// <summary>
	/// Function to update the POI's relative
	/// location on the MiniMap
	/// </summary>
	void UpdateRelativeLocation();

	/// <summary>
	/// Function to calculate the distance
	/// between Character and POI location
	/// </summary>
	/// <returns>Distance vector between Character and POI</returns>
	FVector2D GetDistanceVector();

	/// <summary>
	/// Function to find the angle of distance
	/// between the Character and the POI
	/// </summary>
	/// <param name="CharacterLocation">Character's location - always (0,0) on MiniMap</param>
	/// <param name="ActorLocation">Point of Interest's location</param>
	/// <returns>Angle of Character and POI distance</returns>
	float FindAngle(FVector2D CharacterLocation, FVector2D ActorLocation);

	/// <summary>
	/// Function to clamp the POI icon's location
	/// on the MiniMap
	/// </summary>
	/// <param name="Radius">Distance length between Character and POI</param>
	/// <param name="Degrees">Distance angle between Character and POI</param>
	/// <returns>The clamped location of the POI</returns>
	FVector2D ClampPOIIconLocation(float Radius, float Degrees);
};
