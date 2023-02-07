// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WizardActor.generated.h"

/**
* Base class for the different Actors
* that can be interacted with in the Game
*/
UCLASS()
class WIZARD_API AWizardActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWizardActor();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/// <summary>
	/// Function to show/hide the Interact Widget
	/// </summary>
	/// <param name="bShowInteractWidget">Whether to show or hide the widget</param>
	void ShowInteractWidget(bool bShowInteractWidget);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/// <summary>
	/// Actor's Point of Interest component:
	/// shows the location of the Actor on the MiniMap
	/// </summary>
	UPROPERTY(VisibleAnywhere)
	class UPointOfInterestComponent* POI;

	/// <summary>
	/// AreaSphere to interact with the Actor
	/// </summary>
	UPROPERTY(VisibleAnywhere)
	class USphereComponent* AreaSphere;

	/// <summary>
	/// Interact widget component
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* InteractComponent;

	/// <summary>
	/// Interact widget for showing the instruction on how
	/// to interact with the Actor
	/// </summary>
	UPROPERTY()
	class UOverheadWidget* InteractWidget;

	/// <summary>
	/// Text to show when interacting with the Actor
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Interaction")
	FString InteractText;

};
