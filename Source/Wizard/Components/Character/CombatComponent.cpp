// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "Net/UnrealNetwork.h"
#include "Sound/SoundCue.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Wizard/WizardTypes/CombatTypes.h"
#include "Wizard/Characters/WizardCharacter.h"
#include "Wizard/Characters/WizardAnimInstance.h"
#include "Wizard/Interfaces/WizardCombatActor.h"
#include "Wizard/GameModes/WizardGameMode.h"
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
	DOREPLIFETIME(UCombatComponent, CombatTarget);
	DOREPLIFETIME(UCombatComponent, Successes);
	DOREPLIFETIME(UCombatComponent, Steps);
	DOREPLIFETIME(UCombatComponent, StepIndex);
	DOREPLIFETIME(UCombatComponent, bInitNextStep);
	DOREPLIFETIME(UCombatComponent, bIsAttacking);
}

void UCombatComponent::InitCombat(const TScriptInterface<IWizardCombatActor>& Target, bool bCharacterAttacking)
{
	if (Character && Character->GetAttribute() && Target) {
		if (Character->GetAttribute()->GetPower() < Target->GetCost() && bCharacterAttacking) {
			ClientNotEnoughPowerMessage();
			Target->SetCanInteract(true);
			return;
		}

		SetSpellIndexes();
		SetSpellSteps();
		CombatTarget = Target;
		bIsAttacking = bCharacterAttacking;
		SetSuccessRate();

		if (Character->HasAuthority() && Character->IsLocallyControlled()) SetupCombat();
	}
}

void UCombatComponent::ClientNotEnoughPowerMessage_Implementation()
{
	Character->PlaySound(CastFailSound);
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

void UCombatComponent::OnRep_CombatTarget()
{
	if (Character && Character->IsLocallyControlled()) {
		if (CombatTarget) {
			SetupCombat();
		}
		else {
			Reset();
		}
	}
}

void UCombatComponent::SetSuccessRate()
{
	if (CombatTarget) {
		if (CombatTarget->GetCombatType() <= ECombat::EC_DarkSpell) {
			SuccessRate = static_cast<float>(Character->GetAttribute()->GetWisdom()) / NumberOfSteps;
		}
		else if (CombatTarget->GetCombatType() == ECombat::EC_Trial) {
			SuccessRate = static_cast<float>(Character->GetAttribute()->GetIntelligence()) / NumberOfSteps;
		}
		else if (bIsAttacking) {
			SuccessRate = static_cast<float>(Character->GetAttribute()->GetOffense()) / NumberOfSteps;
		}
		else {
			SuccessRate = static_cast<float>(Character->GetAttribute()->GetDefense()) / NumberOfSteps;
		}
	}
}

void UCombatComponent::SetupCombat()
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
	CombatTarget = nullptr;
	Successes = 0.f;
	SuccessRate = 0.f;

	if (Character && Character->IsLocallyControlled()) Reset();
}

void UCombatComponent::Reset()
{
	WController = (WController == nullptr && Character) ? Character->GetWizardController() : WController;
	if (WController) {
		WController->ResetHUD();
	}
}

void UCombatComponent::StartCombat()
{
	if (Character) {
		MulticastStartCombat();
		FTimerHandle StartCombatTimer;
		GetWorld()->GetTimerManager().SetTimer(
			StartCombatTimer,
			this,
			&UCombatComponent::SetCurrentSpellStep,
			1.5f
		);
	}
}

void UCombatComponent::StartDarkSpellCombat()
{
	if (!Character || !CombatTarget) return;

	Successes = (CombatTarget->GetHealth() / 2) + 2.f;

	if (bIsAttacking) {
		CalculateCombatAttackResult();
	}
	else {
		CalculateCombatDefendResult();
	}
}

void UCombatComponent::StartGoodSpellCombat()
{
	if (!Character || !CombatTarget) return;

	Successes = (CombatTarget->GetHealth() / 2) + 4.f;

	if (bIsAttacking) {
		CalculateCombatAttackResult();
	}
	else {
		CalculateCombatDefendResult();
	}
}

void UCombatComponent::SetCurrentSpellStep()
{
	if (!Character || !CombatTarget) return;

	if (StepIndex < Steps.Num() - 1) { // set Step
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
	else if (Character->GetAttribute() && Character->GetAction()) { // Combat ends
		StepIndex = -1;
		MulticastResetSpellBar();

		if (bIsAttacking) {
			CalculateCombatAttackResult();
		}
		else {
			CalculateCombatDefendResult();
		}
	}
}

void UCombatComponent::CalculateCombatAttackResult()
{
	Character->GetAttribute()->SpendPower(CombatTarget->GetCost(), EAction::EA_Combat);
	int32 Result = FMath::FloorToInt32<float>(Successes);
	if (Result >= CombatTarget->GetHealth()) { // Success
		Character->PlaySound(SuccessSound);
		MulticastCombatHit();

		if (CombatTarget->GetCombatType() == ECombat::EC_GoodSpell) {
			Character->GetAttribute()->AddGoodSpell(1);
		}
		else if (CombatTarget->GetCombatType() == ECombat::EC_DarkSpell) {
			Character->GetAttribute()->AddDarkSpell(1);
		}

		WGameMode = WGameMode == nullptr ? Cast<AWizardGameMode>(GetWorld()->GetAuthGameMode()) : WGameMode;
		if (WGameMode && CombatTarget->GetCombatType() != ECombat::EC_Trial) {
			WGameMode->BroadcastVictory(Character, CombatTarget);
		}

		CombatTarget->Kill();
	}
	else if (Result > 0) { // Failure with Hit
		CombatTarget->ReceiveDamage(Result);
		MulticastCombatHit();
	}
	else { // Failure without Hit
		CombatTarget->ReceiveDamage(Result);
		MulticastCombatFail();
	}

	StopCombat();
	Character->GetAction()->EndAttack();
}

void UCombatComponent::CalculateCombatDefendResult()
{
	int32 Result = FMath::FloorToInt32<float>(Successes);
	if (Result >= CombatTarget->GetBaseDamage()) { // Success
		MulticastCombatHit();
	}
	else { // Failure
		float EnemyDamage = CombatTarget->GetDamage(Result);
		Character->GetAttribute()->ReceiveDamage(EnemyDamage);
		MulticastCombatFail();
	}

	StopCombat();
	Character->GetAction()->EndDefense();
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

	// Check if user input Symbol Index equals to current Step Symbol Index
	if (WController && SpellIndexes[Input] == Steps[StepIndex]) {
		WController->SetCanCastSpell(false);
		MulticastStepSuccess();
		Successes += SuccessRate;
		bSpellBarShouldUpdate = true;
		bStepWasSuccessful = true;
	}
	else {
		MulticastStepFail();
	}

	StartNextStep();
}

void UCombatComponent::OnRep_Successes()
{
	if (Successes > 0.f) {
		bSpellBarShouldUpdate = true;
		bStepWasSuccessful = true;
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

void UCombatComponent::MulticastResetSpellBar_Implementation()
{
	SpellBar->SetScalarParameterValue(SpellBarParameterValue, 0.f);
}

void UCombatComponent::MulticastStartCombat_Implementation()
{
	Character->SetIsInCombat(true);
	Character->SetIsAttacking(bIsAttacking);

	Character->PlaySound(StartSound);
	FName MontageSection = bIsAttacking ? FName("AttackStart") : FName("DefendStart");
	PlayCombatMontage(MontageSection);
	PlayNiagaraEffect(CastEffect, Character);
}

void UCombatComponent::MulticastStepSuccess_Implementation()
{
	Character->PlaySound(CastSound);
}

void UCombatComponent::MulticastStepFail_Implementation()
{
	Character->PlaySound(CastFailSound);
}

void UCombatComponent::MulticastCombatHit_Implementation()
{
	if (CombatEffectComponent) CombatEffectComponent->Deactivate();

	Character->PlaySound(HitSound);
	FName MontageSection = bIsAttacking ? FName("AttackHit") : FName("DefendSuccess");
	PlayCombatMontage(MontageSection);

	AActor* TargetActor = Cast<AActor>(CombatTarget.GetObject());
	PlayNiagaraEffect(HitEffect, TargetActor);

	Character->SetIsInCombat(false);
}

void UCombatComponent::MulticastCombatFail_Implementation()
{
	if (CombatEffectComponent) CombatEffectComponent->Deactivate();

	Character->PlaySound(FailSound);
	FName MontageSection = bIsAttacking ? FName("AttackFail") : FName("DefendFail");
	PlayCombatMontage(MontageSection);

	Character->SetIsInCombat(false);
}

void UCombatComponent::PlayCombatMontage(FName Section)
{
	Character->PlayMontage(CombatMontage, Section);
}

void UCombatComponent::PlayNiagaraEffect(UNiagaraSystem* Effect, AActor* Target)
{
	if (Effect && Target) {
		CombatEffectComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			Effect,
			Target->GetActorLocation(),
			Target->GetActorRotation()
		);
	}
}
