// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PointOfInterestComponent.generated.h"

/**
* Component class to handle Point of Interest functionality
* on MiniMap for Actors
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WIZARD_API UPointOfInterestComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPointOfInterestComponent();
	friend class AWizardCharacter;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	/// <summary>
	/// Server RPC to setup the POI
	/// on the MiniMap
	/// </summary>
	UFUNCTION(Server, UnReliable)
	void ServerSetupPOI(AActor* Owner);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/// <summary>
	/// Function to update the MiniMap
	/// </summary>
	virtual void UpdateMiniMap();

	/// <summary>
	/// Pointer to the GameState
	/// </summary>
	UPROPERTY()
	class AWizardGameState* WizardGameState;

	/// <summary>
	/// PlayerController pointer
	/// </summary>
	class AWizardPlayerController* WizardController;

	/// <summary>
	/// Array of Actors that are present
	/// on the MiniMap
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_MiniMapActors)
	TArray<AActor*> MiniMapActors;

	UFUNCTION()
	void OnRep_MiniMapActors();

	/// <summary>
	/// Whether or not to clamp the POI icon
	/// to the edge of the MiniMap, or make it
	/// disappear once the POI is far enough
	/// </summary>
	UPROPERTY(Replicated, EditAnywhere, Category = "Point Of Interest")
	bool bIsStatic = false;

	/// <summary>
	/// The image that this POI is going to use
	/// on the MiniMap
	/// </summary>
	UPROPERTY(Replicated, EditAnywhere, Category = "Point Of Interest")
	class UTexture2D* IconImage;

public:
	FORCEINLINE bool GetIsStatic() const { return bIsStatic; }
	FORCEINLINE UTexture2D* GetIconImage() const { return IconImage; }
};
