// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionComponent.h"
#include "Net/UnrealNetwork.h"
#include "Wizard/Characters/WizardCharacter.h"
#include "Wizard/PlayerStates/WizardPlayerState.h"
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

	CurrentDistrict = EDistrict::ED_None;
	CachedDistrict = EDistrict::ED_None;
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
	DOREPLIFETIME(UActionComponent, CachedDistrict);
}

#pragma region Movement
void UActionComponent::SetCachedDistrict(EDistrict District)
{
	ServerCachedDistrict(District);

	Controller = Controller == nullptr ? Cast<AWizardPlayerController>(Character->Controller) : Controller;
	if (Controller && CachedDistrict != EDistrict::ED_None) {
		Controller->ShowHUDTravelPopUp(CachedDistrict);
	}
}

void UActionComponent::OnRep_CachedDistrict()
{
	Controller = Controller == nullptr ? Cast<AWizardPlayerController>(Character->Controller) : Controller;
	if (Controller && CachedDistrict != EDistrict::ED_None) {
		Controller->ShowHUDTravelPopUp(CachedDistrict);
	}
}

void UActionComponent::SetCurrentDistrict(EDistrict District)
{
	ServerCurrentDistrict(District);
	
	Controller = Controller == nullptr ? Cast<AWizardPlayerController>(Character->Controller) : Controller;
	PlayerState = (PlayerState == nullptr && Controller) ? Controller->GetPlayerState<AWizardPlayerState>() : PlayerState;
	if (PlayerState && Controller->HasAuthority()) {
		PlayerState->SpendAction(EAction::EA_Movement);
		Controller->SetHUDCurrentDistrict(CurrentDistrict, true);
	}
}

void UActionComponent::OnRep_CurrentDistrict()
{
	Controller = Controller == nullptr ? Cast<AWizardPlayerController>(Character->Controller) : Controller;
	PlayerState = (PlayerState == nullptr && Controller) ? Controller->GetPlayerState<AWizardPlayerState>() : PlayerState;
	if (PlayerState) {
		PlayerState->SpendAction(EAction::EA_Movement);
		Controller->SetHUDCurrentDistrict(CurrentDistrict, true);
	}
}

void UActionComponent::ServerCurrentDistrict_Implementation(EDistrict District)
{
	ServerCachedDistrict(EDistrict::ED_None);
	CurrentDistrict = District;
}

void UActionComponent::ServerCachedDistrict_Implementation(EDistrict District)
{
	CachedDistrict = District;
}
#pragma endregion
