// Fill out your copyright notice in the Description page of Project Settings.


#include "WizardActor.h"
#include "Net/UnrealNetwork.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"
#include "Wizard/HUD/WizardWidgetClasses/OverheadWidget.h"
#include "Wizard/Components/MiniMap/PointOfInterestComponent.h"

// Sets default values
AWizardActor::AWizardActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	// Creating the Point Of Interest Component
	POI = CreateDefaultSubobject<UPointOfInterestComponent>(TEXT("PointOfInterest"));
	POI->SetIsReplicated(true);

	// Create AreaSphere
	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(RootComponent);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	// Create Interact widget
	InteractComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractWidget"));
	InteractComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWizardActor::BeginPlay()
{
	Super::BeginPlay();
	
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	if (InteractComponent) InteractComponent->SetVisibility(false);

	POI->SetupPOI(this);
}

// Called every frame
void AWizardActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Init Interact widget
	if (InteractWidget == nullptr && InteractComponent) {
		InteractWidget = Cast<UOverheadWidget>(InteractComponent->GetWidget());
		if (InteractWidget) {
			InteractWidget->SetDisplayText(InteractText);
		}
	}
}

void AWizardActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWizardActor, bCanInteract);
}

void AWizardActor::SetCanInteract(bool bIsInteractable)
{
	bCanInteract = bIsInteractable;
	ShowInteractWidget(bIsInteractable);
}

void AWizardActor::ShowInteractWidget(bool bShowInteractWidget)
{
	if (InteractComponent) {
		InteractComponent->SetVisibility(bShowInteractWidget);
	}
}
