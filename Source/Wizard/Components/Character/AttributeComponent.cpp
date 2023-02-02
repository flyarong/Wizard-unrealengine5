// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeComponent.h"
#include "Net/UnrealNetwork.h"
#include "Wizard/Characters/WizardCharacter.h"
#include "Wizard/Controllers/WizardPlayerController.h"

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	Energy = MaxEnergy;
	XP = 0;
}


// Called when the game starts
void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAttributeComponent, Name);
	DOREPLIFETIME(UAttributeComponent, Health);
	DOREPLIFETIME(UAttributeComponent, Power);
	DOREPLIFETIME(UAttributeComponent, XP);
	DOREPLIFETIME(UAttributeComponent, Energy);
	DOREPLIFETIME(UAttributeComponent, Wisdom);
	DOREPLIFETIME(UAttributeComponent, Intelligence);
	DOREPLIFETIME(UAttributeComponent, Combat);
	DOREPLIFETIME(UAttributeComponent, Agility);
}

#pragma region Energy
void UAttributeComponent::SpendEnergy(float Cost)
{
	if (Character && Character->HasAuthority()) {
		int32 Divider = Agility == 0 ? 1 : Agility;
		Energy = (FMath::Clamp(Energy - Cost, 0.f, MaxEnergy)) / Divider;
		UpdateHUDEnergy();
	}
}

void UAttributeComponent::OnRep_Energy()
{
	UpdateHUDEnergy();
}

void UAttributeComponent::UpdateHUDEnergy()
{
	Controller = (Controller == nullptr) ?
		Cast<AWizardPlayerController>(Character->Controller) : Controller;
	if (Controller) {
		Controller->SetHUDEnergy(Energy, MaxEnergy);
	}
}
#pragma endregion

void UAttributeComponent::ServerSpendXP_Implementation(int32 Cost)
{
	if (Character && Character->HasAuthority()) {
		XP = FMath::Clamp(XP - Cost, 0.f, XP);
		UpdateHUDXP();
	}
}

void UAttributeComponent::OnRep_XP()
{
	UpdateHUDXP();
}

void UAttributeComponent::UpdateHUDXP()
{
	Controller = (Controller == nullptr) ?
		Cast<AWizardPlayerController>(Character->Controller) : Controller;
	if (Controller) {
		Controller->SetHUDXP(XP);
	}
}