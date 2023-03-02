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
	Health = MaxHealth;
	Power = MaxPower;
	XP = 20;
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
	DOREPLIFETIME(UAttributeComponent, Defense);
	DOREPLIFETIME(UAttributeComponent, XP);
	DOREPLIFETIME(UAttributeComponent, Power);
	DOREPLIFETIME(UAttributeComponent, Wisdom);
	DOREPLIFETIME(UAttributeComponent, Intelligence);
	DOREPLIFETIME(UAttributeComponent, Offense);
	DOREPLIFETIME(UAttributeComponent, Agility);
	DOREPLIFETIME(UAttributeComponent, GoodSpells);
	DOREPLIFETIME(UAttributeComponent, DarkSpells);
}


void UAttributeComponent::OnRep_Health()
{

}

#pragma region Power
void UAttributeComponent::SpendPower(float Cost, EAction ActionType)
{
	if (Character && Character->HasAuthority()) {
		int32 Subtrahend = Cost;
		switch (ActionType)
		{
		case EAction::EA_Movement:
			Subtrahend = Agility == 0 ? Cost : Cost / Agility;
			Power = (FMath::Clamp(Power - Subtrahend, 0.f, MaxPower));
			break;
		case EAction::EA_Combat:
			Power = (FMath::Clamp(Power - Subtrahend, 0.f, MaxPower));
			break;
		default:
			break;
		}
		UpdateHUDPower();
	}
}

void UAttributeComponent::AddPower(int32 AmountToAdd)
{
	if (Character && Character->HasAuthority()) {
		Power = FMath::Clamp(Power + AmountToAdd, 0.f, MaxPower);
		UpdateHUDPower();
	}
}

void UAttributeComponent::OnRep_Power()
{
	UpdateHUDPower();
}

void UAttributeComponent::UpdateHUDPower()
{
	Controller = (Controller == nullptr) ?
		Cast<AWizardPlayerController>(Character->Controller) : Controller;
	if (Controller) {
		Controller->SetHUDPower(Power, MaxPower);
	}
}
#pragma endregion

#pragma region XP
void UAttributeComponent::SpendXP(int32 Cost)
{
	if (Character && Character->HasAuthority()) {
		XP = FMath::Clamp(XP - Cost, 0.f, XP);
		UpdateHUDXP();
	}
}

void UAttributeComponent::AddXP(int32 AmountToAdd)
{
	if (Character && Character->HasAuthority()) {
		XP += AmountToAdd;
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
#pragma endregion

#pragma region Spells
void UAttributeComponent::AddGoodSpell(int32 GoodSpellAmount)
{
	if (Character && Character->HasAuthority()) {
		GoodSpells += GoodSpellAmount;
		UpdateHUDGoodSpells();
	}
}

void UAttributeComponent::OnRep_GoodSpells()
{
	UpdateHUDGoodSpells();
}

void UAttributeComponent::UpdateHUDGoodSpells()
{
	Controller = (Controller == nullptr) ?
		Cast<AWizardPlayerController>(Character->Controller) : Controller;
	if (Controller) {
		Controller->SetHUDSpells(GoodSpells);
	}
}

void UAttributeComponent::AddDarkSpell(int32 DarkSpellAmount)
{
	if (Character && Character->HasAuthority()) {
		DarkSpells += DarkSpellAmount;
		UpdateHUDDarkSpells();
	}
}

void UAttributeComponent::OnRep_DarkSpells()
{
	UpdateHUDDarkSpells();
}

void UAttributeComponent::UpdateHUDDarkSpells()
{
	Controller = (Controller == nullptr) ?
		Cast<AWizardPlayerController>(Character->Controller) : Controller;
	if (Controller) {
		Controller->SetHUDSpells(DarkSpells, false);
	}
}
#pragma endregion