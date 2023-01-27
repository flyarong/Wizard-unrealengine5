// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionComponent.h"
#include "Net/UnrealNetwork.h"
#include "Wizard/Stores/Store.h"
#include "Wizard/Items/Item.h"
#include "Wizard/Characters/WizardCharacter.h"
#include "Wizard/Components/Character/AttributeComponent.h"
#include "Wizard/Components/Districts/District.h"
#include "Wizard/Controllers/WizardPlayerController.h"

// Sets default values for this component's properties
UActionComponent::UActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UActionComponent::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void UActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UActionComponent, CurrentDistrict);
	DOREPLIFETIME(UActionComponent, CurrentStore);
}

#pragma region Movement
void UActionComponent::SetCurrentDistrict(ADistrict* District)
{
	CurrentDistrict = District;
	
	UpdateHUDCurrentDistrict();
}

void UActionComponent::OnRep_CurrentDistrict()
{
	UpdateHUDCurrentDistrict();
}

void UActionComponent::UpdateHUDCurrentDistrict()
{
	Controller = (Controller == nullptr && Character) ? Cast<AWizardPlayerController>(Character->Controller) : Controller;
	if (Controller && Character->GetAttribute() && CurrentDistrict) {
		Character->GetAttribute()->SpendEnergy(CurrentDistrict->GetCost());
		Controller->SetHUDCurrentDistrict(CurrentDistrict->GetDistrictName());
	}
}
#pragma endregion

void UActionComponent::ShowStoreCatalog()
{
	Controller = (Controller == nullptr && Character) ? Cast<AWizardPlayerController>(Character->Controller) : Controller;
	if (Controller && CurrentStore) {
		Controller->SetHUDStoreCatalog(CurrentStore->GetStoreCatalog());
	}
}