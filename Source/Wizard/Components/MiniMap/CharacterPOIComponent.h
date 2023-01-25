// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PointOfInterestComponent.h"
#include "CharacterPOIComponent.generated.h"

/**
 * Point of Interest Component class for the Character
 * Character handles MiniMap update differently
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WIZARD_API UCharacterPOIComponent : public UPointOfInterestComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void UpdateMiniMap() override;

};
