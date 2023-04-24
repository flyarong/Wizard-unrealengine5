// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "Wizard/Enemy/Enemy.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	EnemyCharacter = Cast<AEnemy>(TryGetPawnOwner());
	if (EnemyCharacter) {
		EnemyCharacterMovement = EnemyCharacter->GetCharacterMovement();
		OnPlayMontageNotifyBegin.AddDynamic(EnemyCharacter, &AEnemy::OnMontageNotifyBegin);
	}
}
