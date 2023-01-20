// Fill out your copyright notice in the Description page of Project Settings.


#include "PointOfInterestWidget.h"
#include "MiniMapWidget.h"
#include "Components/Image.h"
#include "Components/Throbber.h"
#include "Wizard/Controllers/WizardPlayerController.h"
#include "Wizard/HUD/WizardHUD.h"
#include "Wizard/HUD/WizardWidgetClasses/WizardOverlay.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Wizard/Components/MiniMap/PointOfInterestComponent.h"

void UPointOfInterestWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Init custom POI image
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

void UPointOfInterestWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Init MiniMap member variable
	if (MiniMap == nullptr) {
		AWizardPlayerController* PlayerController = Cast<AWizardPlayerController>(GetWorld()->GetFirstPlayerController());
		if (PlayerController) {
			AWizardHUD* WizardHUD = Cast<AWizardHUD>(PlayerController->GetHUD());
			if (WizardHUD) {
				MiniMap = WizardHUD->GetOverlay()->GetMiniMap();
			}
		}
	}

	// Update POI's relative location on MiniMap
	if (MiniMap) {
		UpdateRelativeLocation();
	}
}

void UPointOfInterestWidget::UpdateRelativeLocation()
{
	FVector2D Distance = GetDistanceVector(); // Distance between character and point of interest
	FVector2D CharacterMiniMapCoord(0.f, 0.f); // Character is always at MiniMap center
	float Degrees = FindAngle(CharacterMiniMapCoord, Distance);
	FVector2D RenderTranslation = ClampPOIIconLocation(Distance.Size(), Degrees);
	SetRenderTranslation(RenderTranslation);

	// Hide POI image on MiniMap, if it is at the edge and not static
	if (!bIsStatic && RenderTranslation.Size() >= MiniMap->MiniMapRadius) {
		POIImage->SetVisibility(ESlateVisibility::Hidden);	
	}
	else {
		POIImage->SetVisibility(ESlateVisibility::Visible);
	}
}

FVector2D UPointOfInterestWidget::GetDistanceVector()
{
	float Ratio = MiniMap->Zoom * (MiniMap->Dimensions / MiniMap->MiniMapSize);
	FVector CharacterLocation = UGameplayStatics::GetPlayerCharacter(this, 0)->GetActorLocation();
	FVector OwnerLocation = Owner->GetActorLocation();

	return FVector2D(
		(CharacterLocation.X - OwnerLocation.X) / Ratio,
		(-1 * (CharacterLocation.Y - OwnerLocation.Y)) / Ratio
	);
}

float UPointOfInterestWidget::FindAngle(FVector2D CharacterLocation, FVector2D ActorLocation)
{
	float Adjecant = CharacterLocation.X - ActorLocation.X;
	float Opposite = CharacterLocation.Y - ActorLocation.Y;
	return UKismetMathLibrary::Atan2(Adjecant, Opposite); // Need the tan inverse 
}

FVector2D UPointOfInterestWidget::ClampPOIIconLocation(float Radius, float Degrees)
{
	return FVector2D(
		UKismetMathLibrary::DegCos(Degrees) * FMath::Clamp(Radius, 0.f, MiniMap->MiniMapRadius),
		UKismetMathLibrary::DegSin(Degrees) * FMath::Clamp(Radius, 0.f, MiniMap->MiniMapRadius)
	);
}
