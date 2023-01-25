// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Wizard/WizardTypes/DistrictNames.h"
#include "WizardCharacter.generated.h"

UCLASS(Blueprintable)
class AWizardCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AWizardCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;

	/// <summary>
	/// Function for initializing the character's properties
	/// </summary>
	/// <param name="PlayerName">Player's name</param>
	/// <param name="RowName">Selected character's name</param>
	void InitGameplayCharacter(FString PlayerName, FName RowName);

private:

	/// <summary>
	/// Player controller pointer
	/// </summary>
	UPROPERTY()
	class AWizardPlayerController* PlayerController;

#pragma region CharacterComponents
	/// <summary>
	/// Mesh for holding the Magic Staff
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MagicStaff;

	/// <summary>
	/// Overhead widget for showing the player's name
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* OverheadWidget;

	/// <summary>
	/// Character's Attribute component: handling
	/// all the functionality regarding character
	/// attributes (e.g. health)
	/// </summary>
	UPROPERTY(VisibleAnywhere)
	class UAttributeComponent* Attribute;

	/// <summary>
	/// Character's Action component: handling
	/// all the actions a character can do
	/// </summary>
	UPROPERTY(VisibleAnywhere)
	class UActionComponent* Action;

	/// <summary>
	/// Character's Point of Interest component:
	/// shows the location of the Character on the MiniMap
	/// </summary>
	UPROPERTY(VisibleAnywhere)
	class UCharacterPOIComponent* POI;
#pragma endregion

public:
	FORCEINLINE UActionComponent* GetAction() const { return Action; }
	FORCEINLINE UAttributeComponent* GetAttribute() const { return Attribute; }
	FORCEINLINE AWizardPlayerController* GetWizardController() { return PlayerController; }
};

