// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "InteractComponent.generated.h"

/**
 * Widget Component for handling the
 * Interact widget for each Gameplay Actor
 */
UCLASS()
class WIZARD_API UInteractComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:

	/// <summary>
	/// Widget class to create the InteractWidget from
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Interaction")
	TSubclassOf<class UUserWidget> OverheadClass;

	/// <summary>
	/// Text to show when interacting with the Actor
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Interaction")
	FString InteractText;
};
