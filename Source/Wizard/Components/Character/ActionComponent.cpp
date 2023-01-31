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
	DOREPLIFETIME(UActionComponent, bCanBrowse);
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

#pragma region Buying
void UActionComponent::SetCurrentStore(AStore* Store)
{
	CurrentStore = Store;
	if (Character && Character->IsLocallyControlled() && CurrentStore) {
		CurrentStore->ShowInteractWidget(true);
	}
}

void UActionComponent::OnRep_CurrentStore(AStore* PreviousStore)
{
	if (Character && Character->IsLocallyControlled() && CurrentStore) {
		CurrentStore->ShowInteractWidget(true);
	}
	else if (PreviousStore) {
		PreviousStore->ShowInteractWidget(false);
	}
}

void UActionComponent::OpenCatalog()
{
	bCanBrowse = true;

	ShowStoreCatalog();
}

void UActionComponent::OnRep_CanBrowse()
{
	ShowStoreCatalog();
}

void UActionComponent::ShowStoreCatalog()
{
	if (bCanBrowse) {
		Controller = (Controller == nullptr && Character) ? Cast<AWizardPlayerController>(Character->Controller) : Controller;
		if (Controller && CurrentStore) {
			Controller->SetHUDStoreCatalog(CurrentStore->GetStoreCatalog());
		}
	}
}

void UActionComponent::CloseCatalog()
{
	bCanBrowse = false;
}

void UActionComponent::LeaveStore()
{
	if (Character && Character->IsLocallyControlled() && CurrentStore) {
		CurrentStore->ShowInteractWidget(false);
	}
	bCanBrowse = false;
	CurrentStore = nullptr;
}

void UActionComponent::BuyItem(FItemDataTable ItemRow)
{
	if (Character && Character->GetAttribute()) {
		if (Character->GetAttribute()->HasEnoughXP(ItemRow.Cost)) {
			Character->GetAttribute()->ServerSpendXP(ItemRow.Cost);
			Character->AddNewItem(ItemRow);
		}
		else {
			// TODO Client RPC to notify
			if (GEngine) {
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("you don't have enough xp")));
			}
		}
	}
}
#pragma endregion