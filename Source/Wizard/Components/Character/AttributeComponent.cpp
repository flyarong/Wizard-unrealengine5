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

int32 UAttributeComponent::GetAttributeValue(EAttribute AttributeType)
{
	switch (AttributeType)
	{
	case EAttribute::EA_Health:
		return Health;
	case EAttribute::EA_Defense:
		return Defense;
	case EAttribute::EA_Power:
		return Power;
	case EAttribute::EA_XP:
		return XP;
	case EAttribute::EA_Wisdom:
		return Wisdom;
	case EAttribute::EA_Intelligence:
		return Intelligence;
	case EAttribute::EA_Offense:
		return Offense;
	case EAttribute::EA_Agility:
		return Agility;
	}

	return 0;
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

#pragma region Health
void UAttributeComponent::ReceiveDamage(float Damage)
{
	if (Character && Character->HasAuthority()) {
		Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
		UpdateHUDHealth();
	}
}

void UAttributeComponent::AddHealth(int32 AmountToAdd)
{
	if (Character && Character->HasAuthority()) {
		Health = FMath::Clamp(Health + AmountToAdd, 0.f, MaxHealth);
		UpdateHUDHealth();
	}
}

void UAttributeComponent::OnRep_Health()
{
	UpdateHUDHealth();
}

void UAttributeComponent::UpdateHUDHealth()
{
	Controller = (Controller == nullptr && Character) ?
		Cast<AWizardPlayerController>(Character->Controller) : Controller;
	if (Controller) {
		Controller->SetHUDHealth(Health, MaxHealth);
	}
}
#pragma endregion

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
		if (Power <= 0.f) Character->ServerInterruptMovement();
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
	Controller = (Controller == nullptr && Character) ?
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
		XP = FMath::Clamp(XP - Cost, 0, XP);
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
	Controller = (Controller == nullptr && Character) ?
		Cast<AWizardPlayerController>(Character->Controller) : Controller;
	if (Controller) {
		Controller->SetHUDXP(XP);
	}
}
#pragma endregion

#pragma region Spells
void UAttributeComponent::SpendGoodSpell(int32 Cost)
{
	if (Character && Character->HasAuthority()) {
		GoodSpells = FMath::Clamp(GoodSpells - Cost, 0, GoodSpells);
		UpdateHUDGoodSpells();
	}
}

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
	Controller = (Controller == nullptr && Character) ?
		Cast<AWizardPlayerController>(Character->Controller) : Controller;
	if (Controller) {
		Controller->SetHUDSpells(GoodSpells);
	}
}

void UAttributeComponent::SpendDarkSpell(int32 Cost)
{
	if (Character && Character->HasAuthority()) {
		DarkSpells = FMath::Clamp(DarkSpells - Cost, 0, DarkSpells);
		float DarkSpellCost = Intelligence > 1 ? MaxHealth / 2 / Intelligence : MaxHealth / 2;
		ReceiveDamage(DarkSpellCost);
		UpdateHUDDarkSpells();
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
	Controller = (Controller == nullptr && Character) ?
		Cast<AWizardPlayerController>(Character->Controller) : Controller;
	if (Controller) {
		Controller->SetHUDSpells(DarkSpells, false);
	}
}
#pragma endregion

#pragma region Wisdom
void UAttributeComponent::DecreaseWisdom(int32 AmountToSubtract)
{
	if (Character && Character->HasAuthority()) {
		Wisdom = FMath::Clamp(Wisdom - AmountToSubtract, 0, Wisdom);
		UpdateHUDWisdom();
	}
}

void UAttributeComponent::IncreaseWisdom(int32 AmountToAdd)
{
	if (Character && Character->HasAuthority()) {
		Wisdom += AmountToAdd;
		UpdateHUDWisdom();
	}
}

void UAttributeComponent::OnRep_Wisdom()
{
	UpdateHUDWisdom();
}

void UAttributeComponent::UpdateHUDWisdom()
{
	Controller = (Controller == nullptr && Character) ?
		Cast<AWizardPlayerController>(Character->Controller) : Controller;
	if (Controller) {
		Controller->SetHUDWisdom(Wisdom);
	}
}
#pragma endregion

#pragma region Intelligence
void UAttributeComponent::DecreaseIntelligence(int32 AmountToSubtract)
{
	if (Character && Character->HasAuthority()) {
		Intelligence = FMath::Clamp(Intelligence - AmountToSubtract, 0, Intelligence);
		UpdateHUDIntelligence();
	}
}

void UAttributeComponent::IncreaseIntelligence(int32 AmountToAdd)
{
	if (Character && Character->HasAuthority()) {
		Intelligence += AmountToAdd;
		UpdateHUDIntelligence();
	}
}

void UAttributeComponent::OnRep_Intelligence()
{
	UpdateHUDIntelligence();
}

void UAttributeComponent::UpdateHUDIntelligence()
{
	Controller = (Controller == nullptr && Character) ?
		Cast<AWizardPlayerController>(Character->Controller) : Controller;
	if (Controller) {
		Controller->SetHUDIntelligence(Intelligence);
	}
}
#pragma endregion

#pragma region Offense
void UAttributeComponent::DecreaseOffense(int32 AmountToSubtract)
{
	if (Character && Character->HasAuthority()) {
		Offense = FMath::Clamp(Offense - AmountToSubtract, 0, Offense);
		UpdateHUDOffense();
	}
}

void UAttributeComponent::IncreaseOffense(int32 AmountToAdd)
{
	if (Character && Character->HasAuthority()) {
		Offense += AmountToAdd;
		UpdateHUDOffense();
	}
}

void UAttributeComponent::OnRep_Offense()
{
	UpdateHUDOffense();
}

void UAttributeComponent::UpdateHUDOffense()
{
	Controller = (Controller == nullptr && Character) ?
		Cast<AWizardPlayerController>(Character->Controller) : Controller;
	if (Controller) {
		Controller->SetHUDOffense(Offense);
	}
}
#pragma endregion

#pragma region Defense
void UAttributeComponent::DecreaseDefense(int32 AmountToSubtract)
{
	if (Character && Character->HasAuthority()) {
		Defense = FMath::Clamp(Defense - AmountToSubtract, 0, Defense);
		UpdateHUDDefense();
	}
}

void UAttributeComponent::IncreaseDefense(int32 AmountToAdd)
{
	if (Character && Character->HasAuthority()) {
		Defense += AmountToAdd;
		UpdateHUDDefense();
	}
}

void UAttributeComponent::OnRep_Defense()
{
	UpdateHUDDefense();
}

void UAttributeComponent::UpdateHUDDefense()
{
	Controller = (Controller == nullptr && Character) ?
		Cast<AWizardPlayerController>(Character->Controller) : Controller;
	if (Controller) {
		Controller->SetHUDDefense(Defense);
	}
}
#pragma endregion

#pragma region Agility
void UAttributeComponent::DecreaseAgility(int32 AmountToSubtract)
{
	if (Character && Character->HasAuthority()) {
		Agility = FMath::Clamp(Agility - AmountToSubtract, 0, Agility);
		UpdateHUDAgility();
	}
}

void UAttributeComponent::IncreaseAgility(int32 AmountToAdd)
{
	if (Character && Character->HasAuthority()) {
		Agility += AmountToAdd;
		UpdateHUDAgility();
	}
}

void UAttributeComponent::OnRep_Agility()
{
	UpdateHUDAgility();
}

void UAttributeComponent::UpdateHUDAgility()
{
	Controller = (Controller == nullptr && Character) ?
		Cast<AWizardPlayerController>(Character->Controller) : Controller;
	if (Controller) {
		Controller->SetHUDAgility(Agility);
	}
}
#pragma endregion