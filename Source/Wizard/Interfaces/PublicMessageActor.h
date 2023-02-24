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

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual class UTexture2D* GetIcon();
};
