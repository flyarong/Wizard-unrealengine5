// Fill out your copyright notice in the Description page of Project Settings.


#include "WizardAnimInstance.h"
#include "WizardCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UWizardAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	WizardCharacter = Cast<AWizardCharacter>(TryGetPawnOwner());
	if (WizardCharacter) {
		WizardCharacterMovement = WizardCharacter->GetCharacterMovement();
		OnPlayMontageNotifyBegin.AddDynamic(WizardCharacter, &AWizardCharacter::OnMontageNotifyBegin);
	}
}

void UWizardAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (WizardCharacter == nullptr) {
		WizardCharacter = Cast<AWizardCharacter>(TryGetPawnOwner());
		if (WizardCharacter) {
			WizardCharacterMovement = WizardCharacter->GetCharacterMovement();
		}
	}

	if (WizardCharacter) {
		bIsInCombat = WizardCharacter->GetIsInCombat();
		bIsAttacking = WizardCharacter->GetIsAttacking();
	}

	if (WizardCharacterMovement)
	{
		// we only need the X and Y vectors for ground speed
		GroundSpeed = UKismetMathLibrary::VSizeXY(WizardCharacterMovement->Velocity);
	}
}
