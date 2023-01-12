// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MagicStaff;

	/// <summary>
	/// Overhead widget for showing the player's name
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* OverheadWidget;

	/// <summary>
	/// Character's Action component: handling
	/// all the actions a character can do
	/// </summary>
	UPROPERTY(VisibleAnywhere)
	class UActionComponent* Action;
#pragma endregion

#pragma region WizardStats
	UPROPERTY(EditAnywhere, Category = "Wizard Stats")
	FString Name;

	UPROPERTY(EditAnywhere, Category = "Wizard Stats")
	int32 Health;

	UPROPERTY(EditAnywhere, Category = "Wizard Stats")
	int32 Power;
#pragma endregion

public:
	FORCEINLINE UActionComponent* GetAction() { return Action; }
	FORCEINLINE AWizardPlayerController* GetWizardController() { return PlayerController; }
};

