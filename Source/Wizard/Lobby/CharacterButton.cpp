// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterButton.h"
#include "Net/UnrealNetwork.h"

void UCharacterButton::OnSelectButtonClicked() {
	CharacterSelectedDelegate.Broadcast(this);
}
