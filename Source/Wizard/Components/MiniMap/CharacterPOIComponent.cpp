// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterPOIComponent.h"
#include "Wizard/Controllers/WizardPlayerController.h"

void UCharacterPOIComponent::BeginPlay()
{
	UActorComponent::BeginPlay();
}

void UCharacterPOIComponent::UpdateMiniMap()
{
	WizardController = WizardController == nullptr ?
		Cast<AWizardPlayerController>(GetWorld()->GetFirstPlayerController()) : WizardController;
	if (WizardController) {
		if (MiniMapActors.Num() > 0) {
			for (AActor* Owner : MiniMapActors)
			{
				WizardController->SetHUDPOIOnMiniMap(Owner);
			}
		}
	}
}
