// Fill out your copyright notice in the Description page of Project Settings.


#include "PointOfInterestComponent.h"
#include "Wizard/Controllers/WizardPlayerController.h"
#include "Wizard/GameModes/WizardGameMode.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UPointOfInterestComponent::UPointOfInterestComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UPointOfInterestComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UPointOfInterestComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPointOfInterestComponent, IconImage);
	DOREPLIFETIME(UPointOfInterestComponent, bIsStatic);
	DOREPLIFETIME(UPointOfInterestComponent, MiniMapActors);
}

// Called every frame
void UPointOfInterestComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPointOfInterestComponent::SetupPOI(AActor* Owner)
{
	ServerSetupPOI(Owner);
	UpdateMiniMap();
}

void UPointOfInterestComponent::ServerSetupPOI_Implementation(AActor* Owner)
{
	WizardGameMode = WizardGameMode == nullptr ? Cast<AWizardGameMode>(UGameplayStatics::GetGameMode(this)) : WizardGameMode;
	if (WizardGameMode) {
		WizardGameMode->AddMiniMapActor(Owner);
		MiniMapActors = WizardGameMode->GetMiniMapActors();
	}
}

void UPointOfInterestComponent::OnRep_MiniMapActors()
{
	UpdateMiniMap();
}

void UPointOfInterestComponent::UpdateMiniMap()
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
