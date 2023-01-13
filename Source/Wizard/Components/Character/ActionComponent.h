// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Wizard/WizardTypes/DistrictNames.h"
#include "ActionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WIZARD_API UActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActionComponent();
	friend class AWizardCharacter;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	/// <summary>
	/// Pointer to the player character
	/// </summary>
	UPROPERTY()
	class AWizardCharacter* Character;

	/// <summary>
	/// Player controller pointer
	/// </summary>
	UPROPERTY()
	class AWizardPlayerController* Controller;

	/// <summary>
	/// Player state pointer
	/// </summary>
	UPROPERTY()
	class AWizardPlayerState* PlayerState;

#pragma region WizardStats
	UPROPERTY(EditAnywhere, Category = "Wizard Stats")
	int32 Wisdom;

	UPROPERTY(EditAnywhere, Category = "Wizard Stats")
	int32 Intelligence;

	UPROPERTY(EditAnywhere, Category = "Wizard Stats")
	int32 Combat;

	UPROPERTY(EditAnywhere, Category = "Wizard Stats")
	int32 Agility;
#pragma endregion

	/// <summary>
	/// District the player is currently at
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Movement Action", ReplicatedUsing = OnRep_CurrentDistrict)
	EDistrict CurrentDistrict;

	UFUNCTION()
	void OnRep_CurrentDistrict();

public:
	FORCEINLINE void SetWisdom(int32 WisdomToSet) { Wisdom = WisdomToSet; }
	FORCEINLINE void SetIntelligence(int32 IntelligenceToSet) { Intelligence = IntelligenceToSet; }
	FORCEINLINE void SetCombat(int32 CombatToSet) { Combat = CombatToSet; }
	FORCEINLINE void SetAgility(int32 AgilityToSet) { Agility = AgilityToSet; }
	FORCEINLINE EDistrict GetCurrentDistrict() const { return CurrentDistrict; }
	void SetCurrentDistrict(EDistrict District);
};
