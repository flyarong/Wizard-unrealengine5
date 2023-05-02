// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionComponent.h"
#include "Net/UnrealNetwork.h"
#include "Sound/SoundCue.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Wizard/Stores/Store.h"
#include "Wizard/Trials/Trial.h"
#include "Wizard/GameModes/WizardGameMode.h"
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
		if (Character->GetAttribute()->HasEnoughXP(ItemRow.Cost) && !Character->HasEquipment(ItemRow)) {
			Character->GetAttribute()->SpendXP(ItemRow.Cost);
			Character->AddNewItem(ItemRow);
			ClientPlaySound(SuccessfulPurchaseSound);
		}
		else if (!Character->GetAttribute()->HasEnoughXP(ItemRow.Cost)) {
			ClientAddLocalMessage(FString(TEXT("You don't have enough")), EAttribute::EA_XP);
			ClientPlaySound(FailedPurchaseSound);
		}
		else if (Character->HasEquipment(ItemRow)) {
			ClientAddLocalMessage(FString(TEXT("You already have this equipment")), EAttribute::EA_MAX);
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
void UActionComponent::ServerInitAttackCombat_Implementation()
{
	if (Character && Character->GetCombat() && OverlappedWizardActor && !bInCombat) {
		bInCombat = true; // TODO can be used for checking this state when implementing item switching between Characters
		AActor* CombatTargetActor = Cast<AActor>(OverlappedWizardActor.GetObject());
		if (CombatTargetActor) {
			OverlappedWizardActor->SetCanInteract(false);
			MulticastAimCharacterToTarget(CombatTargetActor);
			Character->GetCombat()->InitCombat(CombatTargetActor);
		}
	}
}

void UActionComponent::SetAttacker(AActor* NewAttacker)
{
	if (NewAttacker && !Attackers.Contains(NewAttacker)) Attackers.Insert(NewAttacker, 0);
}

void UActionComponent::InitDefenseCombat(AActor* Attacker)
{
	OverlappedWizardActor = nullptr;

	if (Character && Character->GetCombat() && Attacker && Attacker == Attackers.Last()) { // can't attack Character being attacked by other Enemy
		MulticastAimCharacterToTarget(Attacker);
		bInCombat = true;
		Character->GetCombat()->InitCombat(Attacker, false);
	}
}

void UActionComponent::InitTrialCombat(ATrial* Trial)
{
	OverlappedWizardActor = nullptr;

	if (Character && Character->GetCombat() && Trial) {
		bInCombat = true;
		Character->GetCombat()->InitCombat(Trial);
	}
}

void UActionComponent::ServerCancelCombat_Implementation()
{
	if (Character && Character->GetCombat()) {
		bInCombat = false;
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

void UActionComponent::ServerStartDarkSpellCombat_Implementation()
{
	if (Character && Character->GetCombat() && 
		Character->GetAttribute() && Character->GetAttribute()->GetDarkSpells() > 0) {
		Character->GetAttribute()->SpendDarkSpell(1);
		Character->GetCombat()->StartDarkSpellCombat();
	}
}

void UActionComponent::ServerStartGoodSpellCombat_Implementation()
{
	if (Character && Character->GetCombat() && 
		Character->GetAttribute() && Character->GetAttribute()->GetGoodSpells() > 0) {
		Character->GetAttribute()->SpendGoodSpell(1);
		Character->GetCombat()->StartGoodSpellCombat();
	}
}

void UActionComponent::ServerValidateCombatInput_Implementation(int32 Input)
{
	if (Character && Character->GetCombat()) {
		Character->GetCombat()->StopCurrentTimer();
		Character->GetCombat()->ValidateInput(Input);
	}
}

void UActionComponent::EndAttack()
{
	bInCombat = false;

	if (OverlappedWizardActor) OverlappedWizardActor->SetCanInteract(true);
}

void UActionComponent::EndDefense()
{
	AWizardGameMode* WGameMode = Cast<AWizardGameMode>(GetWorld()->GetAuthGameMode());
	if (WGameMode && Character && Character->GetCombat()) {
		bInCombat = false;
		OnEnemyAttackEndedDelegate.Broadcast(Attackers.Last());
		Attackers.Pop();
		if (Attackers.Num() > 0) { // if there are still Attackers waiting to attack
			OnDefenseCombatEndedDelegate.Broadcast(Character, Attackers.Last());
		}
		WGameMode->IncrementEnemiesFinished();
	}
}

void UActionComponent::MulticastAimCharacterToTarget_Implementation(AActor* Target)
{
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Character->GetActorLocation(), Target->GetActorLocation());
	Character->K2_SetActorRotation(FRotator(Character->GetActorRotation().Pitch, LookAtRotation.Yaw, Character->GetActorRotation().Roll), false);
	if (Target->ActorHasTag("Enemy")) // Rotate Enemy to face Character during Combat
		Target->K2_SetActorRotation(FRotator(Target->GetActorRotation().Pitch, LookAtRotation.Yaw + 180.f, Target->GetActorRotation().Roll), false);
}
#pragma endregion