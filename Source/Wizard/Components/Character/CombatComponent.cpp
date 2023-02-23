// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "Net/UnrealNetwork.h"
#include "Sound/SoundCue.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Wizard/Characters/WizardCharacter.h"
#include "Wizard/Actors/WizardActor.h"
#include "Wizard/Controllers/WizardPlayerController.h"
#include "Wizard/Components/Character/ActionComponent.h"
#include "Wizard/Components/Character/AttributeComponent.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	SpellInputs = {
		EKeys::Up,
		EKeys::Left,
		EKeys::Down,
		EKeys::Right
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
	DOREPLIFETIME(UCombatComponent, CombatTarget);
	DOREPLIFETIME(UCombatComponent, SuccessRate);
	DOREPLIFETIME(UCombatComponent, Successes);
	DOREPLIFETIME(UCombatComponent, Steps);
	DOREPLIFETIME(UCombatComponent, StepIndex);
	DOREPLIFETIME(UCombatComponent, bInitNextStep);
}

void UCombatComponent::InitCombat(int32 AttributeForCombat, AWizardActor* Target)
{
	if (Character && Character->GetAttribute() && Target) {
		if (Character->GetAttribute()->GetPower() < Target->GetCost()) {
			ClientNotEnoughPowerMessage();
			return;
		}

		SetSpellIndexes();
		SetSpellSteps();
		CombatAttribute = AttributeForCombat;
		CombatTarget = Target;
		SuccessRate = CombatAttribute / NumberOfSteps;
		if (Character->HasAuthority() && Character->IsLocallyControlled()) SetupCombatHUD();
	}
}

void UCombatComponent::ClientNotEnoughPowerMessage_Implementation()
{
	WController = (WController == nullptr && Character) ? Character->GetWizardController() : WController;
	if (WController) {
		WController->AddHUDLocalMessage(FString(TEXT("You don't have enough")), EAttribute::EA_Power);
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

void UCombatComponent::SetSpellSteps()
{
	if (SpellIndexes.Num() > 1) {
		for (int32 i = 0; i < NumberOfSteps; i++) {
			int32 Index = FMath::RandRange(0, SpellIndexes.Num() - 1);
			Steps.Add(SpellIndexes[Index]);
		}
	}
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
	if (Steps.Num() > 0) Steps = TArray<int32>();
	if (SpellIndexes.Num() > 0) SpellIndexes = TArray<int32>();
	CombatAttribute = 0.f;
	CombatTarget = nullptr;
	SuccessRate = 0.f;

	if (Character && Character->IsLocallyControlled()) ResetHUD();
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
	if (Character && Character->GetAttribute() && CombatTarget) {
		Character->GetAttribute()->SpendPower(CombatTarget->GetCost());
		MulticastPlayEffect(StartSound, CastEffect);
		FTimerHandle StartCombatTimer;
		GetWorld()->GetTimerManager().SetTimer(
			StartCombatTimer,
			this,
			&UCombatComponent::SetCurrentSpellStep,
			1.5f
		);
	}
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
			StepTime
		);
	}
	else {
		StepIndex = -1;
		MulticastResetSpellBar();

		// If Combat succeeds, destroy Target
		int32 Result = FMath::FloorToInt32<float>(Successes);
		if (CombatTarget && Result >= CombatTarget->GetHealth()) {
			MulticastPlayEffect(SuccessSound);
			MulticastDestroyEffect();
			MulticastPlayEffect(HitSound, HitEffect);
			CombatTarget->Destroy();
		}
		else {
			MulticastPlayEffect(FailSound);
			MulticastDestroyEffect();
		}

		if (Character && Character->GetAction()) {
			Character->GetAction()->EndCombat();
		}
	}
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
		WController->AddHUDCurrentSpellStep(Steps[StepIndex]);
		WController->SetCanCastSpell(true);
	}
}

void UCombatComponent::StartNextStep()
{
	if (StepIndex >= 0) {
		bInitNextStep = true;
		InitNextStep();
	}

	FTimerHandle NextStepTimer;
	GetWorld()->GetTimerManager().SetTimer(
		NextStepTimer,
		this,
		&UCombatComponent::SetCurrentSpellStep,
		1.5f
	);
}

void UCombatComponent::OnRep_InitNextStep()
{
	InitNextStep();
}

void UCombatComponent::InitNextStep()
{
	if (bInitNextStep) {
		RemovePreviousStep();
		bStepWasSuccessful = false;
	}
}

void UCombatComponent::RemovePreviousStep()
{
	WController = (WController == nullptr && Character) ? Character->GetWizardController() : WController;
	if (WController) {
		WController->SetCanCastSpell(false);
		WController->RemoveHUDPreviouseSpellStep();
		WController->AddHUDSpellStepResult(bStepWasSuccessful);
		WController->AddHUDCombatScore(FMath::FloorToInt32<float>(Successes));
	}
}

void UCombatComponent::StopCurrentTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(CurrentStepTimer);
}

void UCombatComponent::ValidateInput(int32 Input)
{
	WController = (WController == nullptr && Character) ? Character->GetWizardController() : WController;
	if (WController) {
		WController->SetCanCastSpell(false);
	}

	// Check if user input Symbol Index equals to current Step Symbol Index
	if (SpellIndexes[Input] == Steps[StepIndex]) {
		MulticastPlayEffect(CastSound);
		Successes += SuccessRate;
		bSpellBarShouldUpdate = true;
		bStepWasSuccessful = true;
	}
	else {
		MulticastPlayEffect(CastFailSound);
	}

	StartNextStep();
}

void UCombatComponent::OnRep_Successes()
{
	bSpellBarShouldUpdate = true;
	bStepWasSuccessful = true;
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

void UCombatComponent::MulticastResetSpellBar_Implementation()
{
	SpellBar->SetScalarParameterValue(SpellBarParameterValue, 0.f);
}

void UCombatComponent::MulticastPlayEffect_Implementation(USoundCue* Sound, UNiagaraSystem* Effect = nullptr)
{
	if (Sound) {
		UGameplayStatics::PlaySoundAtLocation(
			this,
			Sound,
			Character->GetActorLocation()
		);
	}

	if (Effect) {
		CombatEffectComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			Effect,
			Character->GetActorLocation(),
			Character->GetActorRotation()
		);
	}
}

void UCombatComponent::MulticastDestroyEffect_Implementation()
{
	if (CombatEffectComponent) CombatEffectComponent->Deactivate();
}