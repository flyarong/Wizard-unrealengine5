// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerIconWidget.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

void UPlayerIconWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Update Character's rotation on MiniMap
	FRotator CharacterRotation = UGameplayStatics::GetPlayerCharacter(this, 0)->GetMesh()->GetComponentRotation();
	PlayerIcon->SetRenderTransformAngle(90.f + CharacterRotation.Yaw);
}