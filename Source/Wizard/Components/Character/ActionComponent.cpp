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

	// ...
}


// Called when the game starts
void UActionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
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

	DOREPLIFETIME(UActionComponent, CurrentDistrict)
}

#pragma region Movement
void UActionComponent::SetCurrentDistrict(EDistrict District)
{
	CurrentDistrict = District;

	Controller = Controller == nullptr ? Cast<AWizardPlayerController>(Character->Controller) : Controller;
	PlayerState = PlayerState == nullptr ? Controller->GetPlayerState<AWizardPlayerState>() : PlayerState;
	if (Controller && PlayerState) {
		PlayerState->SpendAction(EAction::EA_Movement);
		Controller->SetHUDCurrentDistrict(CurrentDistrict, true);
	}
}

void UActionComponent::OnRep_CurrentDistrict()
{
	Controller = Controller == nullptr ? Cast<AWizardPlayerController>(Character->Controller) : Controller;
	PlayerState = PlayerState == nullptr ? Controller->GetPlayerState<AWizardPlayerState>() : PlayerState;
	if (Controller && PlayerState) {
		PlayerState->SpendAction(EAction::EA_Movement);
		Controller->SetHUDCurrentDistrict(CurrentDistrict, true);
	}
}
#pragma endregion
