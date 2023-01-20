// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MiniMapWidget.generated.h"

/**
 * Widget class for the MiniMap
 */
UCLASS()
class WIZARD_API UMiniMapWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	/// <summary>
	/// The MiniMap image
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "MiniMap")
	class UTexture* Image;

	/// <summary>
	/// How much of the map should be seen on the MiniMap
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "MiniMap")
	float Zoom;
	
	/// <summary>
	/// Distance the Map's image was taken from
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "MiniMap")
	float Dimensions;
	
	/// <summary>
	/// Size of the MiniMap
	/// </summary>
	UPROPERTY()
	float MiniMapSize = 0.f;
	
	/// <summary>
	/// Radius of the rounded MiniMap
	/// </summary>
	UPROPERTY()
	float MiniMapRadius = 0.f;

	/// <summary>
	/// Function to add a Player of Interest (POI)
	/// to the MiniMap
	/// </summary>
	/// <param name="Owner">POI's Owner</param>
	void AddPOI(class AActor* Owner);

private:

	/// <summary>
	/// Function to add the player's icon to the MiniMap
	/// </summary>
	void AddPlayerIcon();

	UPROPERTY(EditAnywhere, Category = "MiniMap")
	class UMaterialParameterCollection* MiniMapParameterCollection;

	UPROPERTY(EditAnywhere, Category = "MiniMap")
	TSubclassOf<class UUserWidget> PlayerIconWidget;

	UPROPERTY(EditAnywhere, Category = "MiniMap")
	TSubclassOf<class UUserWidget> PointOfInterestWidget;

	UPROPERTY(meta = (BindWidget))
	class UOverlay* MapOverlay;

	/// <summary>
	/// Image of the Map
	/// </summary>
	UPROPERTY(meta = (BindWidget))
	class UImage* MiniMapImage;
};
