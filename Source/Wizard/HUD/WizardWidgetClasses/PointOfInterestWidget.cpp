// Fill out your copyright notice in the Description page of Project Settings.


#include "PointOfInterestWidget.h"
#include "MiniMapWidget.h"
#include "Components/Image.h"
#include "Components/Throbber.h"
#include "Wizard/Controllers/WizardPlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Wizard/HUD/WizardHUD.h"
#include "Wizard/HUD/WizardWidgetClasses/WizardOverlay.h"
#include "Kismet/GameplayStatics.h"
#include "Wizard/Pawns/GameplayCamera.h"
#include "Kismet/KismetMathLibrary.h"
#include "Wizard/Components/MiniMap/PointOfInterestComponent.h"

void UPointOfInterestWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Init custom POI image
	if (Owner) {
		UPointOfInterestComponent* POIComponent =
			Cast<UPointOfInterestComponent>(Owner->GetComponentByClass(UPointOfInterestComponent::StaticClass()));
		if (POIComponent) {
			POIImage->SetBrushFromTexture(POIComponent->GetIconImage());
			if (POIImage) {
				DefaultImage->SetVisibility(ESlateVisibility::Hidden);
			}
			else {
				POIImage->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

void UPointOfInterestWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Init MiniMap member variable
	if (MiniMap == nullptr && Owner) {
		AWizardPlayerController* PlayerController = Cast<AWizardPlayerController>(GetWorld()->GetFirstPlayerController());
		if (PlayerController) {
			AWizardHUD* WizardHUD = Cast<AWizardHUD>(PlayerController->GetHUD());
			if (WizardHUD) {
				MiniMap = WizardHUD->GetOverlay()->GetMiniMap();
			}
		}
	}

	// Update POI's relative location on MiniMap
	if (MiniMap && Owner) {
		UpdateRelativeLocation();
	}

	// Delete if Owner is no longer valid
	if (!Owner) {
		RemoveFromParent();
	}
}

void UPointOfInterestWidget::UpdateRelativeLocation()
{
	FVector2D Distance = GetDistanceVector(); // Distance between character and point of interest
	FVector2D CameraMiniMapCoord(0.f, 0.f); // Gameplay Camera is always at MiniMap center
	float Degrees = FindAngle(CameraMiniMapCoord, Distance);
	FVector2D RenderTranslation = ClampPOIIconLocation(Distance.Length(), Degrees);
	SetRenderTranslation(RenderTranslation);

	// Hide POI image on MiniMap, if it is at the edge and not static
	if (!bIsStatic && RenderTranslation.Length() >= MiniMap->MiniMapRadius) {
		POIImage->SetVisibility(ESlateVisibility::Hidden);	
	}
	else {
		POIImage->SetVisibility(ESlateVisibility::Visible);
	}
}

FVector2D UPointOfInterestWidget::GetDistanceVector()
{
	float Ratio = MiniMap->Zoom * (MiniMap->Dimensions / MiniMap->MiniMapSize);
	FVector CameraLocation = UGameplayStatics::GetActorOfClass(this, AGameplayCamera::StaticClass())->GetActorLocation();
	FVector OwnerLocation = Owner->GetActorLocation();

	return FVector2D(
		(CameraLocation.X - OwnerLocation.X) / Ratio,
		(-1 * (CameraLocation.Y - OwnerLocation.Y)) / Ratio
	);
}

float UPointOfInterestWidget::FindAngle(FVector2D CameraLocation, FVector2D ActorLocation)
{
	float Adjecant = CameraLocation.X - ActorLocation.X;
	float Opposite = CameraLocation.Y - ActorLocation.Y;
	return UKismetMathLibrary::Atan2(Adjecant, Opposite); // Need the tan inverse 
}

FVector2D UPointOfInterestWidget::ClampPOIIconLocation(float Radius, float Degrees)
{
	float RadiusBorder = MiniMap->MiniMapRadius * 0.9f;

	return FVector2D(
		-1 * (UKismetMathLibrary::Cos(Degrees) * FMath::Clamp(Radius, 0.f, RadiusBorder)),
		-1 * (UKismetMathLibrary::Sin(Degrees) * FMath::Clamp(Radius, 0.f, RadiusBorder))
	);
}
