// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniMapWidget.h"
#include "PlayerIconWidget.h"
#include "PointOfInterestWidget.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Image.h"
#include "Wizard/Pawns/GameplayCamera.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Wizard/Components/MiniMap/PointOfInterestComponent.h"

void UMiniMapWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Init MiniMap
	UKismetMaterialLibrary::SetScalarParameterValue(this, MiniMapParameterCollection, FName("Zoom"), Zoom);
	UKismetMaterialLibrary::SetScalarParameterValue(this, MiniMapParameterCollection, FName("Dimension"), Dimensions);
	MiniMapImage->GetDynamicMaterial()->SetTextureParameterValue(FName("MiniMapImage"), Image);
	AddPlayerIcon();
}

void UMiniMapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Get MiniMap size
	if (MiniMapSize == 0.f) {
		UCanvasPanelSlot* SizeSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(this);
		MiniMapSize = SizeSlot->GetSize().X;
		MiniMapRadius = MiniMapSize / 2;
	}

	// Update Camera's location on MiniMap
	AActor* GameplayCamera = UGameplayStatics::GetActorOfClass(this, AGameplayCamera::StaticClass());
	if (GameplayCamera) {
		FVector CameraLocation = GameplayCamera->GetActorLocation();
		UKismetMaterialLibrary::SetScalarParameterValue(this, MiniMapParameterCollection, FName("X"), CameraLocation.X);
		UKismetMaterialLibrary::SetScalarParameterValue(this, MiniMapParameterCollection, FName("Y"), CameraLocation.Y);
		UKismetMaterialLibrary::SetScalarParameterValue(this, MiniMapParameterCollection, FName("Zoom"), Zoom);
	}
}

void UMiniMapWidget::AddPlayerIcon()
{
	UPlayerIconWidget* PlayerIcon = CreateWidget<UPlayerIconWidget>(this, PlayerIconWidget);
	if (PlayerIcon) {
		UOverlaySlot* PlayerIconSlot = MapOverlay->AddChildToOverlay(PlayerIcon);
		if (PlayerIconSlot) {
			PlayerIconSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
			PlayerIconSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
		}
	}
}

void UMiniMapWidget::AddPOI(AActor* Owner)
{
	UPointOfInterestWidget* POIIcon = CreateWidget<UPointOfInterestWidget>(this, PointOfInterestWidget);
	if (POIIcon) {
		POIIcon->Owner = Owner;
		UPointOfInterestComponent* POIComponent =
			Cast<UPointOfInterestComponent>(Owner->GetComponentByClass(UPointOfInterestComponent::StaticClass()));
		if (POIComponent) {
			POIIcon->bIsStatic =POIComponent->GetIsStatic();
			UOverlaySlot* POISlot = MapOverlay->AddChildToOverlay(POIIcon);
			if (POISlot) {
				POISlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
				POISlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
			}
		}
	}
}
