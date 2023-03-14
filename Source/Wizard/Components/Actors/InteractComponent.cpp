// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractComponent.h"
#include "Wizard/HUD/WizardWidgetClasses/OverheadWidget.h"

void UInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	UOverheadWidget* InteractWidget = CreateWidget<UOverheadWidget>(GetWorld()->GetFirstPlayerController(), OverheadClass);
	if (InteractWidget) {
		InteractWidget->SetDisplayText(InteractText);
		SetWidget(InteractWidget);
	}
}
