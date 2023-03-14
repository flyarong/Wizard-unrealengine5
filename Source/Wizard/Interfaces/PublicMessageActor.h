// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PublicMessageActor.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPublicMessageActor : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for Public Messages
 */
class WIZARD_API IPublicMessageActor
{
	GENERATED_BODY()

public:

	virtual class UTexture2D* GetIcon() = 0;
};
