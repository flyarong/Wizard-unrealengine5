// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "Net/UnrealNetwork.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Wizard/Characters/WizardCharacter.h"
#include "Wizard/Controllers/WizardPlayerController.h"
#include "Wizard/Components/Character/ActionComponent.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	SpellInputs = {
		EKeys::W,
		EKeys::A,
		EKeys::S,
		EKeys::D
	};
}

// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateSpellBar(DeltaTime);
}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCombatComponent, SpellInputs);
	DOREPLIFETIME(UCombatComponent, SpellIndexes);
	DOREPLIFETIME(UCombatComponent, CombatAttribute);
	DOREPLIFETIME(UCombatComponent, Steps);
	DOREPLIFETIME(UCombatComponent, StepIndex);
	DOREPLIFETIME(UCombatComponent, bSpellBarShouldUpdate);
	DOREPLIFETIME(UCombatComponent, bInitNextStep);
}

void UCombatComponent::InitCombat(int32 AttributeForCombat)
{
	SetSpellSteps();
	SetSpellIndexes();
	CombatAttribute = AttributeForCombat;
	if (Character && Character->HasAuthority() && Character->IsLocallyControlled()) SetupCombatHUD();
}

void UCombatComponent::OnRep_CombatAttribute()
{
	if (Character && Character->IsLocallyControlled()) {
		if (CombatAttribute > 0) {
			SetupCombatHUD();
		}
		else {
			ResetHUD();
		}
	}
}

void UCombatComponent::SetupCombatHUD()
{
	WController = (WController == nullptr && Character) ? Character->GetWizardController() : WController;
	if (WController) {
		WController->SetInputContext(EInputContext::EIC_Combat);
		WController->CreateHUDSpellMap(SpellInputs, SpellIndexes);
		WController->AddHUDCombatMenu();
	}
}

void UCombatComponent::StopCombat()
{
	if (Steps.Num() > 0) Steps = TArray<FKey>();
	if (SpellIndexes.Num() > 0) SpellIndexes = TArray<int32>();
	CombatAttribute = 0;

	if (Character && Character->HasAuthority() && Character->IsLocallyControlled()) ResetHUD();
}

void UCombatComponent::ResetHUD()
{
	WController = (WController == nullptr && Character) ? Character->GetWizardController() : WController;
	if (WController) {
		WController->SetInputContext(EInputContext::EIC_Default);
		WController->ResetHUD();
	}
}

void UCombatComponent::StartCombat()
{
	FTimerHandle StartCombatTimer;
	GetWorld()->GetTimerManager().SetTimer(
		StartCombatTimer,
		this,
		&UCombatComponent::SetCurrentSpellStep,
		1.5f
	);
}

void UCombatComponent::OnRep_StepIndex()
{
	if (StepIndex > -1)	AddCurrentStep();
}

void UCombatComponent::AddCurrentStep()
{
	WController = (WController == nullptr && Character) ? Character->GetWizardController() : WController;
	if (WController) {
		if (StepIndex == 0) WController->AddHUDSpellMap();
		int32 SpellIndex = -1;
		for (int32 i = 0; i < SpellInputs.Num(); i++) {
			if (Steps[StepIndex] == SpellInputs[i]) {
				SpellIndex = i;
				break;
			}
		}
		WController->AddHUDCurrentSpellStep(SpellIndex);
		WController->SetCanCastSpell(true);
	}
}

void UCombatComponent::OnRep_InitNextStep()
{
	RemovePreviousStep();
}

void UCombatComponent::RemovePreviousStep()
{
	if (bInitNextStep) {
		WController = (WController == nullptr && Character) ? Character->GetWizardController() : WController;
		if (WController) {
			WController->SetCanCastSpell(false);
			WController->RemoveHUDPreviouseSpellStep();
		}
	}
}

void UCombatComponent::SetSpellSteps()
{
	if (SpellInputs.Num() > 1) {
		for (int32 i = 0; i < NumberOfSteps; i++) {
			int32 Index = FMath::RandRange(0, SpellInputs.Num() - 1);
			Steps.Add(SpellInputs[Index]);
		}
	}
}

void UCombatComponent::SetSpellIndexes()
{
	if (SpellInputs.Num() == 4) {
		TArray<int32> CachedIndexes = { 0, 1, 2, 3 };
		for (int32 i = 0; i < SpellInputs.Num(); i++) {
			int32 Index = FMath::RandRange(0, CachedIndexes.Num() - 1);
			SpellIndexes.Add(CachedIndexes[Index]);
			CachedIndexes.RemoveAt(Index);
		}
	}
}

void UCombatComponent::UpdateSpellBar(float DeltaTime)
{
	if (bSpellBarShouldUpdate) {
		ValueThisFrame = Rate * DeltaTime;
		float DeltaValue = FMath::Clamp(SpellBar->K2_GetScalarParameterValue(SpellBarParameterValue) + ValueThisFrame, 0.f, 1.f);
		SpellBar->SetScalarParameterValue(SpellBarParameterValue, DeltaValue);

		Amount -= ValueThisFrame;
		if (Amount <= 0.f) {
			bSpellBarShouldUpdate = false;
			Amount = 1.f / NumberOfSteps;
		}
	}
}

void UCombatComponent::StartNextStep()
{
	if (StepIndex >= 0) {
		bInitNextStep = true;
		RemovePreviousStep();
	}

	FTimerHandle NextStepTimer;
	GetWorld()->GetTimerManager().SetTimer(
		NextStepTimer,
		this,
		&UCombatComponent::SetCurrentSpellStep,
		1.f
	);
}

void UCombatComponent::SetCurrentSpellStep()
{
	if (StepIndex < Steps.Num() - 1) {
		StepIndex++;
		AddCurrentStep();
		bInitNextStep = false;
		GetWorld()->GetTimerManager().SetTimer(
			CurrentStepTimer,
			this,
			&UCombatComponent::StartNextStep,
			1.5f
		);
	}
	else {
		// TODO pass result to playerstate
		if (Character && Character->GetAction()) {
			Character->GetAction()->EndCombat();
		}
	}
}

void UCombatComponent::StopCurrentTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(CurrentStepTimer);
	// TODO check key input and start next step timer
}