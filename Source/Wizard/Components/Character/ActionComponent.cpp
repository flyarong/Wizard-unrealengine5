// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionComponent.h"
#include "Net/UnrealNetwork.h"
#include "Sound/SoundCue.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Wizard/Stores/Store.h"
#include "Wizard/Characters/WizardCharacter.h"
#include "Wizard/Components/Character/AttributeComponent.h"
#include "Wizard/Components/Character/CombatComponent.h"
#include "Wizard/Components/Districts/District.h"
#include "Wizard/Controllers/WizardPlayerController.h"
#include "Wizard/Interfaces/WizardActor.h"
#include "Wizard/Interfaces/WizardCombatActor.h"

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
	DOREPLIFETIME(UActionComponent, OverlappedWizardActor);
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
		Character->GetAttribute()->SpendPower(CurrentDistrict->GetCost(), EAction::EA_Movement);
		Controller->SetHUDCurrentDistrict(CurrentDistrict->GetDistrictName());
	}
}
#pragma endregion

#pragma region Interaction
void UActionComponent::SetCurrentWizardActor(const TScriptInterface<IWizardActor>& WizardActor)
{
	if (OverlappedWizardActor == nullptr) { // can only overlap one WizardActor at a time
		OverlappedWizardActor = WizardActor;
		if (Character && Character->IsLocallyControlled() && OverlappedWizardActor && 
			OverlappedWizardActor->GetCanInteract()) {
			OverlappedWizardActor->ShowInteractWidget(true);
		}
	}
}

void UActionComponent::OnRep_OverlappedWizardActor(const TScriptInterface<IWizardActor>& PreviousWizardActor)
{
	if (Character && Character->IsLocallyControlled()) {
		if (OverlappedWizardActor && OverlappedWizardActor->GetCanInteract()) {
			OverlappedWizardActor->ShowInteractWidget(true);
		}
		else if (PreviousWizardActor) {
			PreviousWizardActor->ShowInteractWidget(false);
		}
	}
}

void UActionComponent::LeaveWizardActor()
{
	if (Character && Character->IsLocallyControlled() && OverlappedWizardActor) {
		OverlappedWizardActor->ShowInteractWidget(false);
	}
	OverlappedWizardActor = nullptr;
}
#pragma endregion

#pragma region Buying
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
	if (bCanBrowse && OverlappedWizardActor) {
		AStore* CurrentStore = Cast<AStore>(OverlappedWizardActor.GetObject());
		Controller = (Controller == nullptr && Character) ? Cast<AWizardPlayerController>(Character->Controller) : Controller;
		if (Controller && CurrentStore) {
			Controller->SetHUDStoreCatalog(CurrentStore);
		}
	}
}

void UActionComponent::CloseCatalog()
{
	bCanBrowse = false;
}

void UActionComponent::ServerBuyItem_Implementation(const FItemDataTable& ItemRow)
{
	if (Character && Character->GetAttribute() && !ItemRow.ItemName.IsEmpty() && OverlappedWizardActor) {
		if (Character->GetAttribute()->HasEnoughXP(ItemRow.Cost)) {
			Character->GetAttribute()->SpendXP(ItemRow.Cost);
			Character->AddNewItem(ItemRow);
			ClientPlaySound(SuccessfulPurchaseSound);
		}
		else {
			ClientAddLocalMessage(FString(TEXT("You don't have enough")), EAttribute::EA_XP);
			ClientPlaySound(FailedPurchaseSound);
		}
	}
}

void UActionComponent::ClientPlaySound_Implementation(USoundCue* Sound)
{
	Character->PlaySound(Sound);
}
#pragma endregion

#pragma region Messaging
void UActionComponent::ClientAddLocalMessage_Implementation(const FString& Message, EAttribute AttributeType)
{
	Controller = (Controller == nullptr && Character) ? Cast<AWizardPlayerController>(Character->Controller) : Controller;
	if (Controller) {
		Controller->AddHUDLocalMessage(Message, AttributeType);
	}
}
#pragma endregion

#pragma region Combat
void UActionComponent::ServerInitCombat_Implementation()
{
	if (Character && Character->GetCombat() && OverlappedWizardActor) {
		AActor* CombatTargetActor = Cast<AActor>(OverlappedWizardActor.GetObject());
		if (CombatTargetActor) {
			OverlappedWizardActor->SetCanInteract(false);
			MulticastAimCharacterToTarget(CombatTargetActor);
			Character->GetCombat()->InitCombat(CombatTargetActor);
		}
	}
}

void UActionComponent::ServerCancelCombat_Implementation()
{
	if (Character && Character->GetCombat()) {
		Character->GetCombat()->StopCombat();
	}

	if (OverlappedWizardActor) OverlappedWizardActor->SetCanInteract(true);
}

void UActionComponent::ServerStartCombat_Implementation()
{
	if (Character && Character->GetCombat()) {
		Character->GetCombat()->StartCombat();
	}
}

void UActionComponent::ServerValidateCombatInput_Implementation(int32 Input)
{
	if (Character && Character->GetCombat()) {
		Character->GetCombat()->StopCurrentTimer();
		Character->GetCombat()->ValidateInput(Input);
	}
}

void UActionComponent::EndCombat()
{
	if (Character && Character->GetCombat()) {
		Character->GetCombat()->StopCombat();
	}

	if (OverlappedWizardActor) OverlappedWizardActor->SetCanInteract(true);
}

void UActionComponent::MulticastAimCharacterToTarget_Implementation(AActor* Target)
{
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Character->GetActorLocation(), Target->GetActorLocation());
	Character->K2_SetActorRotation(FRotator(Character->GetActorRotation().Pitch, LookAtRotation.Yaw, Character->GetActorRotation().Roll), false);
}
#pragma endregion