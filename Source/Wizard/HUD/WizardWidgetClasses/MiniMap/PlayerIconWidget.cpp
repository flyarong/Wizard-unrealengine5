// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerIconWidget.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "Wizard/Pawns/GameplayCamera.h"

void UPlayerIconWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Update Camera's rotation on MiniMap
	FRotator CameraRotation = UGameplayStatics::GetActorOfClass(this, AGameplayCamera::StaticClass())->GetActorRotation();
	PlayerIcon->SetRenderTransformAngle(CameraRotation.Yaw);
}