// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Wizard/Characters/WizardCharacter.h"
#include "Wizard/Controllers/WizardPlayerController.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	if (SpellInputs.Num() > 1) {
		for (int32 i = 0; i < NumberOfSteps; i++) {
			int32 Index = FMath::RandRange(0, SpellInputs.Num() - 1);
			Steps.Add(SpellInputs[Index]);
		}
	}
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

void UCombatComponent::InitCombat()
{
	WController = (WController == nullptr && Character) ? Character->GetWizardController() : WController;
	if (WController) {
		SetSpellMap();
		WController->SetCameraPositionToCombat();
		WController->AddHUDCombatMenu();
		WController->AddHUDSpellMap(SpellMap);
	}
}

void UCombatComponent::SetSpellMap()
{
	if (SpellInputs.Num() > 1 && Symbols.Num() > 1 && SpellInputs.Num() == Symbols.Num()) {
		if (SpellMap.Num() > 0) SpellMap = TMap<FKey, UTexture2D*>();
		TArray<UTexture2D*> CachedSymbols = Symbols;
		for (int32 i = 0; i < SpellInputs.Num(); i++) {
			int32 Index = FMath::RandRange(0, CachedSymbols.Num() - 1);
			SpellMap.Add(SpellInputs[i], CachedSymbols[Index]);
			CachedSymbols.RemoveAt(Index);
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
			Amount = 1.f / (NumberOfSteps + 1);
		}
	}
}
